#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QQmlEngine>

#include "chess24.h"
#include "preparechess24ws.h"
#include "message.h"


//Forward
class TournamentsModel;
class QNetworkAccessManager;
class DiskNetworkCookieJar;

class Chess24Login:public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loggedin READ loggedIn NOTIFY loggedInChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
public:
    Chess24Login(QObject *parent,
                 const QNetworkAccessManager &qnam,
                 Chess24 &c24);
    //QML properties
    QString username();
    QString password();
    bool loggedIn() const;
    void setUsername(QString);
    void setPassword(QString);
    UserData userData() const;


    void start();
private:
    void setLoggedIn(bool);
    bool m_loggedIn = false;
    QString m_username;
    QString m_password;
    UserData m_userData;
    const QNetworkAccessManager &qnam;
    Chess24 &c24;

   signals:
    //Signals to UI
    void usernameChanged();
    void passwordChanged();
    void loggedInChanged();

    //Signals
    void result(UserData);

public slots:
    void loginResult(UserData data);
    void login(QString username, QString password);
    //void handleMessage(Message);
};

#endif // BACKEND_H
