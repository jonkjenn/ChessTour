#include "backend.h"

//View models
#include "tournamentsmodel.h"
#include "tournamentviewmodel.h"

//PGN reader
#include "PGNGameCollection.h"
#include <sstream>
#include <iostream>

//Qt
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickItem>

//Chess24
#include "chess24.h"
#include "chess24websocket.h"
#include "preparechess24ws.h"

Backend::Backend(QObject *parent):QObject(parent), c24ws(parent,qnam), prepc24(parent,qnam)
{
    /*//Chess24 c24;
    QUrl pgnurl("http://theweekinchess.com/assets/files/pgn/chgbr17.pgn");

    std::cout << "Getting pgn" << std::endl;
    QNetworkRequest request(pgnurl);
    request.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla Firefox");
    reply = qnam.get(request);
    connect(reply,&QNetworkReply::finished,this,&Backend::httpFinished);**/


    /*foreach(auto o,engine.rootObjects()){
        qDebug() << o->objectName();
    }*/

    /*QQmlComponent loginComponent(&engine,QUrl(QStringLiteral("qrc:/LoginDialog.qml")));
    foreach(auto error,loginComponent.errors()){
        qDebug() << error;
    }

    QObject *loginObject = loginComponent.create();
    loginObject->setParent(engine.rootObjects().first());
    QQuickItem *loginDialog = qobject_cast<QQuickItem*>(loginObject);
    loginDialog->setParentItem(engine.rootObjects().first());
    qDebug() << engine.rootObjects().first()->objectName();
    qDebug() << loginObject->parent();
    connect(loginObject,SIGNAL(accepted()),this,SLOT(loginAccepted()));


    foreach(auto c, engine.children().first()->children()){
        qDebug() << c->objectName();
    }*/

    //c24.login("test","test");
    //connect(&c24,&Chess24::loginResult,this,&Backend::loggedIn);
    DiskNetworkCookieJar::loadCookieJar(jar);
    qnam.setCookieJar(&jar);
    c24.downloadUserData();
    connect(&c24,&Chess24::loginResult,this,&Backend::loginResult);
    connect(&prepc24,&PrepareChess24WS::failure,[](QString error){
       qDebug() << "Connecting to WS failed";
       qDebug() << error;
    });

    connect(&prepc24,&PrepareChess24WS::success,this,[this](QString username, QString password){
       if(c24ws.isConnected()){
           return;
       }
       c24ws.connectWS(data,username,password);
    });
}

void Backend::setLoggedIn(bool loggedIn)
{
    m_loggedIn = loggedIn;
    emit loggedInChanged();
}

bool Backend::loggedIn()
{
    return m_loggedIn;
}

QString Backend::username()
{
    return m_username;
}

QString Backend::password()
{
    return m_password;
}

void Backend::setUsername(QString username)
{
    m_username = username;
    emit usernameChanged();
}

void Backend::setPassword(QString)
{
    m_password = password();
    emit passwordChanged();
}

void Backend::httpFinished()
{
    std::cout << "Got data" << std::endl;
    QByteArray data = reply->readAll();
    QString txt(data);

    pgn::GameCollection games;

    std::stringstream ss;
    ss << txt.toStdString();
    ss >> games;

    if(games.size()>0){
        pgn::Game g = games[0];
        std::cout << g.moves() << std::endl;
    }
}

void Backend::loginResult(UserData data)
{
    this->data = data;
    if(data.isRegistered){
        qDebug() << "Logged in";
        setLoggedIn(true);
        if(data.loginSource == UserData::LoginSource::USERPASS){
            DiskNetworkCookieJar *jar = static_cast<DiskNetworkCookieJar*>(qnam.cookieJar());
            jar->saveCookieJar();
        }
        prepc24.start();
    }else{
        qDebug() << "Login failed";
        setLoggedIn(false);
    }
}

void Backend::login(QString username, QString password)
{
    //Try with username and password if cookie didnt work
    c24.login(username,password);
}

void Backend::loginAccepted()
{
    qDebug() << "Dialog accepted";
}
