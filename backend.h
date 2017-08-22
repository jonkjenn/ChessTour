#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QQmlEngine>
#include <QtNetwork>

#include "chess24.h"
#include "chess24websocket.h"
#include "preparechess24ws.h"
#include "disknetworkcookiejar.h"

//Forward
class TournamentsModel;

class Backend:public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loggedin READ loggedIn NOTIFY loggedInChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
public:
    bool loggedIn();
    QString username();
    QString password();
    void setUsername(QString);
    void setPassword(QString);
    Backend(QObject *parent, TournamentsModel *tm);
private:
    void setLoggedIn(bool);
    bool m_loggedIn = false;
    QString m_username;
    QString m_password;
    QNetworkReply *reply = nullptr;
    QNetworkAccessManager qnam;
    Chess24 c24{this,qnam};
    Chess24Websocket c24ws;
    PrepareChess24WS prepc24;
    DiskNetworkCookieJar jar;
    UserData data;
    TournamentsModel *tm = nullptr;

   signals:
    void loggedInChanged();
    void usernameChanged();
    void passwordChanged();

public slots:
    void loginResult(UserData data);
    void login(QString username, QString password);
private slots:
    void httpFinished();
    void loginAccepted();
};

#endif // BACKEND_H
