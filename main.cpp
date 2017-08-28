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

#include "tournamentssqlmodel.h"
#include "chess24sqlhandler.h"

#include "chess24login.h"
#include "chess24.h"
#include "chess24messages.h"
#include "chess24messageparser.h"
#include "chess24login.h"
#include "chess24manager.h"

#include "disknetworkcookiejar.h"

/*static QJSValue backendSingleton(QQmlEngine *engine,QJSEngine *jsengine){
    QJSValue backend = engine->newQObject(new Backend(engine));
    return backend;
}*/

using namespace std;

int main(int argc, char *argv[])
{
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

    Chess24Manager *c24Manager = new Chess24Manager(&app,*c24ws);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Tournament");
    db.setDatabaseName(QDir::currentPath() + QDir::separator() + "db.sqlite");
    if(!db.open()){//TODO::Error handling
        qDebug() << "Could not open database";
        return 0;
    }

    TournamentsSqlModel *tsm = new TournamentsSqlModel(&app,db);
    tsm->setTable("Tournament");
    tsm->setEditStrategy(QSqlTableModel::OnFieldChange);
    tsm->select();
    Chess24SqlHandler *c24Sql = new Chess24SqlHandler(&app,db,tsm,*c24Manager);

    /*TournamentsModel *tm = new TournamentsModel(&app,*c24Manager);
    TournamentViewModel *tvm = new TournamentViewModel(&app,false);
    tvm->setSourceModel(tm);
    RoundViewModel *rm = new RoundViewModel(&app);
    rm->setSourceModel(tm);
    MatchViewModel *mv = new MatchViewModel(&app);
    mv->setSourceModel(tm);*/


    //tsm->setRelation(4,QSqlRelation("Players","FideId","Name"));
    //tsm->setRelation(5,QSqlRelation("Players","FideId","Name"));

    //Get the tournament details when select a new tournament
    //QObject::connect(tvm,&TournamentViewModel::currentTournamentChanged,tm,&TournamentsModel::onCurrentTournamentChanged);
    QObject::connect(tsm,&TournamentsSqlModel::currentIndexChanged,c24Sql,&Chess24SqlHandler::onCurrentTournamentChanged);

    //Asumes that everytime someone calls Chess24Manager.getTournaments we want to update the model
    QObject::connect(c24Manager,&Chess24Manager::gotTournaments,c24Sql,&Chess24SqlHandler::addTournaments);
    QObject::connect(c24Manager,&Chess24Manager::gotTournamentDetails,c24Sql,&Chess24SqlHandler::updateTournamentDetails);

    QMetaObject::Connection onConnectedCon;

    QSettings settings;
    QDateTime tourUpdated = settings.value("database/tournamentUpdated").toDateTime();
    if(!tourUpdated.isValid() ||//Setting has never been created
            (tourUpdated.isValid() && tourUpdated.daysTo(QDateTime::currentDateTimeUtc())>=1)){//More then one day since last updated tournaments

            //One shot retrieval of data on first start, not particularly elegant since depends on onConnectedCon alive on stack
            onConnectedCon = QObject::connect(c24ws, &Chess24Websocket::connected,[&onConnectedCon,c24Manager](){//When websocket is connected
            QObject::disconnect(onConnectedCon);//Only want to react once
            c24Manager->getTournaments();});
    }

    QObject::connect(chess24Login,&Chess24Login::loggedInChanged,c24ws,&Chess24Websocket::onLoggedInChanged);
    chess24Login->start();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("chess24Login",chess24Login);
    engine.rootContext()->setContextProperty("tournamentsSqlModel",tsm);
    /*engine.rootContext()->setContextProperty("tournamentViewModel",tvm);
    engine.rootContext()->setContextProperty("roundsModel",rm);
    engine.rootContext()->setContextProperty("matchesModel",mv);*/
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
