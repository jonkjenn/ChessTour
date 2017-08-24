#include "chess24login.h"

//View models
#include "tournamentsmodel.h"
#include "tournamentviewmodel.h"

//PGN reader
//#include "PGNGameCollection.h"
#include <sstream>
#include <iostream>

//Qt
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickItem>

//Chess24
#include "chess24.h"
#include "preparechess24ws.h"
#include "chess24messages.h"
#include "wsrequest.h"
#include "message.h"
#include "disknetworkcookiejar.h"


Chess24Login::Chess24Login(QObject *parent,
                         const TournamentsModel &tm,
                         const QNetworkAccessManager &qnam,
                         Chess24 &c24):
    QObject(parent), tm(tm), qnam(qnam), c24(c24)
{
}

void Chess24Login::start(){

    //Try login from cookie

    qDebug() << "Trying login from cookie";
    connect(&c24,&Chess24::loginResult,this,&Chess24Login::loginResult);
    c24.checkLoggedIn(UserData::LoginSource::COOKIE);

/*    connect(&prepc24,&PrepareChess24WS::failure,[](QString error){
       qDebug() << "Connecting to WS failed";
       qDebug() << error;
    });

    connect(&prepc24,&PrepareChess24WS::success,this,[this](QString username, QString password){
       if(c24ws.isConnected()){
           return;
       }
       c24ws.connectWS(data,username,password);
    });*/
}


void Chess24Login::setLoggedIn(bool loggedIn)
{
    m_loggedIn = loggedIn;
}

bool Chess24Login::loggedIn() const
{
    return m_loggedIn;
}

QString Chess24Login::username()
{
    return m_username;
}

QString Chess24Login::password()
{
    return m_password;
}

void Chess24Login::setUsername(QString username)
{
    m_username = username;
    emit usernameChanged();
}

void Chess24Login::setPassword(QString)
{
    m_password = password();
    emit passwordChanged();
}

UserData Chess24Login::userData() const{
    return m_userData;
}

void Chess24Login::loginResult(UserData data)
{
    bool was_loggedIn = loggedIn();
    m_userData = data;

    if(m_userData.isRegistered){
        qDebug() << "Logged in";
        setLoggedIn(true);
        if(m_userData.loginSource == UserData::LoginSource::USERPASS){
            DiskNetworkCookieJar *jar = static_cast<DiskNetworkCookieJar*>(qnam.cookieJar());
            jar->saveCookieJar();
        }
    }else{
        qDebug() << "Login failed";
        setLoggedIn(false);
    }

    if(loggedIn() != was_loggedIn){
        emit loggedInChanged();
    }
}

void Chess24Login::login(QString username, QString password)
{
    //Try with username and password if cookie didnt work
    c24.login(username,password);
}

/*void Chess24Login::handleMessage(Message msg)
{
}*/
