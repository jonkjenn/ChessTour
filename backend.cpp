#include "backend.h"
#include "tournamentsmodel.h"
#include "PGNGameCollection.h"
#include <sstream>
#include <iostream>
#include "tournamentviewmodel.h"
#include <QQmlApplicationEngine>
#include "chess24.h"
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickItem>

Backend::Backend(QObject *parent):QObject(parent)
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
    if(data.result){
        qDebug() << "Logged in";
        setLoggedIn(true);
    }else{
        qDebug() << "Login failed";
        setLoggedIn(false);
    }
}

void Backend::login()
{
    c24.login("test","test");
    connect(&c24,&Chess24::loginResult,this,&Backend::loginResult);
}

void Backend::loginAccepted()
{
    qDebug() << "Dialog accepted";
}
