#include <memory>

#include <QSettings>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QNetworkAccessManager>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDir>
#include <QTimer>

#include "tournamentssqlmodel.h"
#include "roundssqlmodel.h"
#include "livematchsqlmodel.h"
#include "tokencontainer.h"

#include "chess24sqlhandler.h"
#include "chess24login.h"
#include "chess24.h"
#include "chess24messages.h"
#include "chess24messageparser.h"
#include "chess24login.h"
#include "chess24manager.h"
#include "chess24websocket.h"

#include "disknetworkcookiejar.h"

/*static QJSValue backendSingleton(QQmlEngine *engine,QJSEngine *jsengine){
    QJSValue backend = engine->newQObject(new Backend(engine));
    return backend;
}*/
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}


using namespace std;

#include <QJsonDocument>
#include "Tests/testdata.h"
int main(int argc, char *argv[])
{

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(TestData::webTournamentDiff.toUtf8(),&error);
    QVariant test = doc.toVariant();
    QVariantMap map = test.toMap();

    QVariantMap result = Chess24Messages::transformWebTournament(map.value("diffs").toMap());


    qInstallMessageHandler(myMessageOutput);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("Jonkjenn");
    QCoreApplication::setOrganizationDomain("jonkjenn.com");
    QCoreApplication::setApplicationName("ChessTour");
    QGuiApplication app(argc, argv);

    DiskNetworkCookieJar *jar = new DiskNetworkCookieJar(&app);
    QNetworkAccessManager *qnam = new QNetworkAccessManager(&app);
    qnam->setCookieJar(jar);

    Chess24 *chess24 = new Chess24(&app,*qnam);


    Chess24MessageParser *parser = new Chess24MessageParser(&app);

    Chess24Login *chess24Login = new Chess24Login(&app,*qnam, *chess24);

    PrepareChess24WS *prepc24ws = new PrepareChess24WS(&app,*qnam);
    Chess24Websocket *c24ws = new Chess24Websocket(&app,*qnam,*chess24Login,*prepc24ws);
    QObject::connect(c24ws,&Chess24Websocket::messageReceived,parser,&Chess24MessageParser::parseMessage);
    QObject::connect(parser,&Chess24MessageParser::messageParsed,c24ws,&Chess24Websocket::handleMessage);


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + QDir::separator() + "db.sqlite");
    if(!db.open()){//TODO::Error handling
        qDebug() << "Could not open database";
        return 0;
    }


    TournamentsSqlModel *tsm = new TournamentsSqlModel(&app,db);
    tsm->setTable("Tournament");
    tsm->setEditStrategy(QSqlTableModel::OnFieldChange);
    tsm->select();

    RoundsSqlModel *rsm = new RoundsSqlModel(&app,db);
    rsm->setTable("Round");
    rsm->setEditStrategy(QSqlTableModel::OnFieldChange);

    LiveMatchSqlModel *lsm = new LiveMatchSqlModel(&app,db);

    Chess24SqlHandler *c24Sql = new Chess24SqlHandler(&app);

    QTimer *tournamentTokenTimer = new QTimer(&app);
    tournamentTokenTimer->setInterval(60*1000);
    tournamentTokenTimer->start();

    QTimer *tournamentListTokenTimer = new QTimer(&app);
    tournamentListTokenTimer->setInterval(60*1000);
    tournamentListTokenTimer->start();

    TokenContainer *tournamentToken = new TokenContainer(&app,*tournamentTokenTimer,1,1);
    TokenContainer *tournamentListToken = new TokenContainer(&app,*tournamentListTokenTimer,1,1);

    Chess24Manager *c24Manager = new Chess24Manager(&app,*c24ws,*c24Sql,*tsm,*rsm,*lsm,*tournamentToken,*tournamentListToken);

    //Get the tournament details when select a new tournament
    QObject::connect(tsm,&TournamentsSqlModel::currentIndexChanged,c24Manager,&Chess24Manager::onCurrentTournamentChanged);
    QObject::connect(parser,&Chess24MessageParser::webTournamentRedisAR,c24Manager,&Chess24Manager::onWebTournamentRedisAR);

    QMetaObject::Connection onConnectedCon;

    QSettings settings;
    QDateTime tourUpdated = settings.value("database/tournamentUpdated").toDateTime();
    if(!tourUpdated.isValid() ||//Setting has never been created
            (tourUpdated.isValid() && tourUpdated.daysTo(QDateTime::currentDateTimeUtc())>=1)){//More then one day since last updated tournaments

        //One shot retrieval of data on first start, not particularly elegant since depends on onConnectedCon alive on stack
        onConnectedCon = QObject::connect(c24ws, &Chess24Websocket::connected,[&onConnectedCon,c24Manager](){//When websocket is connected
            QObject::disconnect(onConnectedCon);//Only want to react once
            c24Manager->refreshTournamentList();
        });
    }

    QObject::connect(chess24Login,&Chess24Login::loggedInChanged,c24ws,&Chess24Websocket::onLoggedInChanged);
    chess24Login->start();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("chess24Login",chess24Login);
    engine.rootContext()->setContextProperty("tournamentsSqlModel",tsm);
    engine.rootContext()->setContextProperty("roundSqlModel",rsm);
    engine.rootContext()->setContextProperty("matchSqlModel",lsm);
    engine.rootContext()->setContextProperty("c24Manager",c24Manager);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    /*using namespace Chess24Messages;
    QVariantList tournaments = Chess24Messages::tournamentNamesFromJSON(dat);
    tm->addTournaments(tournaments);*/

    //    qDebug() << "After adding";

    //    QTimer *timer = new QTimer(&app);
    //    timer->setSingleShot(true);
    //    timer->setInterval(2000);

    //    QObject::connect(timer,&QTimer::timeout,[tvm](){
    //        tvm->sort(0);
    //    //});

    //timer->start();

    return app.exec();
}
