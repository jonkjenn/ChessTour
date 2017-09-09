#ifndef CHESS24MESSAGEPARSER_H
#define CHESS24MESSAGEPARSER_H
#include <QObject>
#include <QJsonObject>
#include "message.h"
#include "chess24sqlhandler.h"

struct WebTournamentRedisAR{
    QString tournament;
    QJsonObject args;
};

class Chess24MessageParser:public QObject
{
    Q_OBJECT
public:
    Chess24MessageParser(QObject *,Chess24SqlHandler &sqlHandler);

    void parseEvent(const QString &model, const QJsonObject &root);
signals:
     void messageParsed(Message);
     //void webTournamentRedisAR(WebTournamentRedisAR);
     void tournamentMatchUpdates(int tournamentPk, const QVariantList &rounds);

   public slots:
    void parseMessage(QString msg);
private:
    Chess24SqlHandler &sqlHandler;
};

#endif // CHESS24MESSAGEPARSER_H
