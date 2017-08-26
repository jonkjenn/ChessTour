#include <memory>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QNetworkAccessManager>

#include "tournamentviewmodel.h"
#include "tournamentsmodel.h"
#include "roundviewmodel.h"
#include "matchViewModel.h"

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


    TournamentsModel *tm = new TournamentsModel(&app,*c24Manager);
    TournamentViewModel *tvm = new TournamentViewModel(&app,false);
    tvm->setSourceModel(tm);
    RoundViewModel *rm = new RoundViewModel(&app);
    rm->setSourceModel(tm);
    MatchViewModel *mv = new MatchViewModel(&app);
    mv->setSourceModel(tm);

    //Get the tournament details when select a new tournament
    QObject::connect(tvm,&TournamentViewModel::currentTournamentChanged,tm,&TournamentsModel::onCurrentTournamentChanged);

    //Asumes that everytime someone calls Chess24Manager.getTournaments we want to update the model
    QObject::connect(c24Manager,&Chess24Manager::gotTournaments,tm,&TournamentsModel::addTournaments);
    QObject::connect(c24Manager,&Chess24Manager::gotTournamentDetails,tm,&TournamentsModel::updateTournamentDetails);

    //One shot retrieval of data on first start, not particularly elegant
    QMetaObject::Connection onConnectedCon = QObject::connect(c24ws, &Chess24Websocket::connected,[&onConnectedCon,c24Manager](){//When websocket is connected
        QObject::disconnect(onConnectedCon);//Only want to react once
        c24Manager->getTournaments();
    });

    QObject::connect(chess24Login,&Chess24Login::loggedInChanged,c24ws,&Chess24Websocket::onLoggedInChanged);
    chess24Login->start();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("chess24Login",chess24Login);
    engine.rootContext()->setContextProperty("tournamentViewModel",tvm);
    engine.rootContext()->setContextProperty("roundsModel",rm);
    engine.rootContext()->setContextProperty("matchesModel",mv);
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
