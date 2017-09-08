#ifndef CHESS24MESSAGEPARSER_H
#define CHESS24MESSAGEPARSER_H
#include <QObject>
#include <QJsonObject>
#include "message.h"

struct WebTournamentRedisAR{
    QString tournament;
    QJsonObject args;
};

class Chess24MessageParser:public QObject
{
    Q_OBJECT
public:
    Chess24MessageParser(QObject *);


    void parseEvent(const QString &model, const QJsonObject &root);
signals:
     void messageParsed(Message);
     void webTournamentRedisAR(WebTournamentRedisAR);

   public slots:
    void parseMessage(QString msg);
};

#endif // CHESS24MESSAGEPARSER_H
