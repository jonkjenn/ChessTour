#include "chess24login.h"

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


LoginViewModel::LoginViewModel(QObject *parent)
    :QObject(parent)
{
}

void LoginViewModel::start(){
    //Try login from cookie
    qDebug() << "Trying login from cookie";
    emit checkLoggedIn(UserData::LoginSource::COOKIE);
}


void LoginViewModel::setLoggedIn(bool loggedIn)
{
    m_loggedIn = loggedIn;
}

bool LoginViewModel::loggedIn() const
{
    return m_loggedIn;
}

QString LoginViewModel::username()
{
    return m_username;
}

QString LoginViewModel::password()
{
    return m_password;
}

void LoginViewModel::setUsername(QString username)
{
    m_username = username;
    emit usernameChanged();
}

void LoginViewModel::setPassword(QString)
{
    m_password = password();
    emit passwordChanged();
}

UserData LoginViewModel::userData() const{
    return m_userData;
}

void LoginViewModel::onLoginResult(UserData data)
{
    bool was_loggedIn = loggedIn();
    m_userData = data;

    if(m_userData.isRegistered){
        qDebug() << "Logged in";
        setLoggedIn(true);
    }else{
        qDebug() << "Login failed";
        setLoggedIn(false);
    }

    if(loggedIn() != was_loggedIn){
        emit loggedInChanged(loggedIn());
        emit userDataChanged(data);
    }
}

void LoginViewModel::login(QString username, QString password)
{
    //Try with username and password if cookie didnt work
    emit tryLogin(username,password);
}
