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
#include "tournamentcontroller.h"

#include "chess24sqlhandler.h"
#include "chess24login.h"
#include "chess24.h"
#include "chess24messages.h"
#include "chess24messageparser.h"
#include "chess24login.h"
#include "chess24manager.h"
#include "chess24websocket.h"

#include "disknetworkcookiejar.h"

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


#include <QJsonDocument>
#include "Tests/testdata.h"
int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("Jonkjenn");
    QCoreApplication::setOrganizationDomain("jonkjenn.com");
    QCoreApplication::setApplicationName("ChessTour");
    QGuiApplication app(argc, argv);

    //Cookie to be used for logging into chess24
    DiskNetworkCookieJar *jar = new DiskNetworkCookieJar(&app);
    QNetworkAccessManager *qnam = new QNetworkAccessManager(&app);
    qnam->setCookieJar(jar);

    Chess24 *chess24 = new Chess24(&app,*qnam);
    Chess24MessageParser *parser = new Chess24MessageParser(&app);
    Chess24Login *chess24Login = new Chess24Login(&app,*qnam, *chess24);
    PrepareChess24WS *prepc24ws = new PrepareChess24WS(&app,*qnam);
    Chess24Websocket *c24ws = new Chess24Websocket(&app,*qnam,*chess24Login,*prepc24ws);

    //All received websocket messages goes to messageparser
    QObject::connect(c24ws,&Chess24Websocket::messageReceived,parser,&Chess24MessageParser::parseMessage);
    //Chess24websocket uses som of the parsed messages
    QObject::connect(parser,&Chess24MessageParser::messageParsed,c24ws,&Chess24Websocket::handleMessage);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + QDir::separator() + "db.sqlite");
    if(!db.open()){//TODO::Error handling
        qDebug() << "Could not open database";
        return 0;
    }


    //View models
    TournamentsSqlModel *tsm = new TournamentsSqlModel(&app,db);
    tsm->setTable("Tournament");
    tsm->setEditStrategy(QSqlTableModel::OnFieldChange);
    tsm->select();

    RoundsSqlModel *rsm = new RoundsSqlModel(&app,db);
    rsm->setTable("Round");
    rsm->setEditStrategy(QSqlTableModel::OnFieldChange);

    LiveMatchSqlModel *lsm = new LiveMatchSqlModel(&app,db);
    //View models end

    Chess24SqlHandler *c24Sql = new Chess24SqlHandler(&app);
    Chess24Manager *c24Manager = new Chess24Manager(&app,*c24ws,*c24Sql,*tsm,*rsm,*lsm);

    //Token guards
    QTimer *tournamentTokenTimer = new QTimer(&app);
    tournamentTokenTimer->setInterval(60*1000);
    tournamentTokenTimer->start();

    QTimer *tournamentListTokenTimer = new QTimer(&app);
    tournamentListTokenTimer->setInterval(60*1000);
    tournamentListTokenTimer->start();

    TokenContainer *tournamentToken = new TokenContainer(&app,*tournamentTokenTimer,1,1);
    TokenContainer *tournamentListToken = new TokenContainer(&app,*tournamentListTokenTimer,1,1);
    //End token guards

    TournamentController *tc = new TournamentController(&app,*c24Manager,*tsm,*tournamentToken,*tournamentListToken,*c24Sql);

    //React to tournament updates
    QObject::connect(parser,&Chess24MessageParser::webTournamentRedisAR,c24Manager,&Chess24Manager::onWebTournamentRedisAR);

    //Get the tournament details when select a new tournament
    QObject::connect(tsm,&TournamentsSqlModel::currentTournamentChanged,tc,&TournamentController::onCurrentTournamentChanged);
    //Change round when a new tournament is loaded
    QObject::connect(tc,&TournamentController::tournamentLoaded,rsm,&RoundsSqlModel::onTournamentLoaded);
    //Change match when a round changes
    QObject::connect(rsm,&RoundsSqlModel::roundChanged,lsm,&LiveMatchSqlModel::onCurrentRoundChanged);

    //On startup we want to retrieve or refresh the list of tournaments, unless it recently has been updated
    QMetaObject::Connection onConnectedCon;//Used to react to a signal only one time

    //Settings from disk
    QSettings settings;
    QDateTime tourUpdated = settings.value("database/tournamentUpdated").toDateTime();

    if(!tourUpdated.isValid() ||//Setting has never been created
            (tourUpdated.isValid() && tourUpdated.daysTo(QDateTime::currentDateTimeUtc())>=1)){//More then one day since last updated tournaments

        //One shot retrieval of data on first start, not particularly elegant since depends on onConnectedCon alive on stack
        onConnectedCon = QObject::connect(c24ws, &Chess24Websocket::connected,[&onConnectedCon,tc](){//When websocket is connected
            QObject::disconnect(onConnectedCon);//Only want to react once
            tc->refreshTournamentList();
        });
    }
    //End refresh tournament list

    QObject::connect(chess24Login,&Chess24Login::loggedInChanged,c24ws,&Chess24Websocket::onLoggedInChanged);
    chess24Login->start();//Will try login from cookie

    //Add stuff to QML
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("chess24Login",chess24Login);
    engine.rootContext()->setContextProperty("tournamentsSqlModel",tsm);
    engine.rootContext()->setContextProperty("tournamentController",tc);
    engine.rootContext()->setContextProperty("roundSqlModel",rsm);
    engine.rootContext()->setContextProperty("matchSqlModel",lsm);
    engine.rootContext()->setContextProperty("c24Manager",c24Manager);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
