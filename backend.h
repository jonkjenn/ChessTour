#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include "tournamentsmodel.h"
#include <QtNetwork>
#include "chess24.h"
#include <QQmlApplicationEngine>

class BackEnd:public QObject
{
    Q_OBJECT
public:
    BackEnd(QObject *);
private:
    TournamentsModel tm{this};
    QNetworkReply *reply;
    QNetworkAccessManager qnam;
    Chess24 c24{this,qnam};
    QQmlApplicationEngine engine;

private slots:
    void httpFinished();
    void loggedIn(UserData);
};

#endif // BACKEND_H
