#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <QNetworkAccessManager>
#include <QObject>

#include "userdata.h"


class LoginManager:public QObject
{
    Q_OBJECT
public:
    LoginManager(QObject *parent):QObject(parent){}
public slots:
    virtual void onCheckLoggedIn(UserData::LoginSource source)=0;
    virtual void onTryLogin(QString username, QString password)=0;
signals:
    void loginResult(UserData);

};

#endif // LOGINMANAGER_H
