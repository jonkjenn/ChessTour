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
#include <QThread>
#include <QWebSocket>

#include "tournamentssqlmodel.h"
#include "roundssqlmodel.h"
#include "livematchsqlmodel.h"
#include "tokencontainer.h"
#include "tournamentcontroller.h"
#include "datamanager.h"

#include "chess24sqlhandler.h"
#include "chess24login.h"
#include "chess24.h"
#include "chess24messages.h"
#include "chess24messageparser.h"
#include "chess24login.h"
#include "chess24manager.h"
#include "chess24websocket.h"
#include "preparechess24ws.h"

#include "chesshelper.h"
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
        return;
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


    TournamentController *tc = new TournamentController(&app,*tsm,*tournamentToken,*tournamentListToken);

    qRegisterMetaType<UserData>();
    qRegisterMetaType<UserData::LoginSource>();
    qRegisterMetaType<InternalMessages::RoundChangedData>();
    qRegisterMetaType<InternalMessages::TournamentChangedData>();

    LoginViewModel *chess24Login = new LoginViewModel(&app);

    QThread *workerThread = new QThread(&app);

    //Cookie to be used for logging into chess24
    DiskNetworkCookieJar *jar = new DiskNetworkCookieJar(nullptr);
    jar->moveToThread(workerThread);
    QNetworkAccessManager *qnam = new QNetworkAccessManager();
    qnam->setCookieJar(jar);
    qnam->moveToThread(workerThread);

    LoginManager *loginManager = new Chess24(nullptr,*qnam);
    loginManager->moveToThread(workerThread);
    PrepareChess24WS *prepc24ws = new PrepareChess24WS(nullptr,*qnam);
    prepc24ws->moveToThread(workerThread);


    //Chess24Websocket
    QTimer *websocketTokenTimer = new QTimer(nullptr);
    websocketTokenTimer->setInterval(2000);
    websocketTokenTimer->start();
    TokenContainer *websocketToken = new TokenContainer(nullptr,*websocketTokenTimer,2,1);

    QTimer *msgQTimer = new QTimer(nullptr);
    msgQTimer->setInterval(1000);
    msgQTimer->start();

    QWebSocket *ws = new QWebSocket();

    Chess24Websocket *c24ws = new Chess24Websocket(nullptr,*ws,*websocketToken,*msgQTimer);
    c24ws->moveToThread(workerThread);
    QObject::connect(prepc24ws,&PrepareChess24WS::success,c24ws,&Chess24Websocket::connectWS);
    //End Chess24Websocket


    Chess24Manager *c24Manager = new Chess24Manager(nullptr,*c24ws);
    c24Manager->moveToThread(workerThread);

    Chess24SqlHandler *c24Sql = new Chess24SqlHandler(nullptr);
    c24Sql->moveToThread(workerThread);
    Chess24MessageParser *parser = new Chess24MessageParser(nullptr,*c24Sql);
    parser->moveToThread(workerThread);
    //All received websocket messages goes to messageparser
    QObject::connect(c24ws,&Chess24Websocket::messageReceived,parser,&Chess24MessageParser::parseMessage);
    //Chess24websocket uses som of the parsed messages
    QObject::connect(parser,&Chess24MessageParser::messageParsed,c24ws,&Chess24Websocket::handleMessage);

    DataManager *dm = new DataManager(nullptr,*c24Manager,*c24Sql);
    dm->moveToThread(workerThread);

    //React to tournament match updates
    QObject::connect(parser,&Chess24MessageParser::tournamentMatchUpdates,lsm,&LiveMatchSqlModel::possibleUpdates);

    //Get the tournament details when select a new tournament
    QObject::connect(tsm,&TournamentsSqlModel::currentTournamentChanged,dm,&DataManager::onCurrentTournamentChanged);
    //Change round when a new tournament is loaded
    QObject::connect(dm,&DataManager::tournamentLoaded,rsm,&RoundsSqlModel::onTournamentLoaded);
    //React to refresh buttons
    QObject::connect(tc,&TournamentController::requestUpdateTournament,dm,&DataManager::onRequestUpdateTournament);
    QObject::connect(tc,&TournamentController::requestUpdateTournamentList,dm,&DataManager::onRequestUpdateTournamentList);
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

    QObject::connect(chess24Login,&LoginViewModel::userDataChanged,c24ws,&Chess24Websocket::onUserdataChanged);
    QObject::connect(chess24Login,&LoginViewModel::userDataChanged,prepc24ws,&PrepareChess24WS::onUserDataChanged);
    QObject::connect(chess24Login,&LoginViewModel::tryLogin,loginManager,&LoginManager::onTryLogin);
    QObject::connect(chess24Login,&LoginViewModel::checkLoggedIn,loginManager,&LoginManager::onCheckLoggedIn);
    QObject::connect(loginManager,&LoginManager::loginResult,chess24Login,&LoginViewModel::onLoginResult);


    workerThread->start();
    QObject::connect(workerThread,&QThread::started,[chess24Login]{
        chess24Login->start();//Will try login from cookie
    });

    ChessHelper *ch = new ChessHelper(&app);

    //Add stuff to QML
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("chess24Login",chess24Login);
    engine.rootContext()->setContextProperty("tournamentsSqlModel",tsm);
    engine.rootContext()->setContextProperty("tournamentController",tc);
    engine.rootContext()->setContextProperty("roundSqlModel",rsm);
    engine.rootContext()->setContextProperty("matchSqlModel",lsm);
    engine.rootContext()->setContextProperty("chessHelper",ch);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
