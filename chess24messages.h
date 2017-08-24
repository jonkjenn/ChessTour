#ifndef CHESS24MESSAGES_H
#define CHESS24MESSAGES_H

#include "chess24.h"

namespace Chess24Messages
{
    QString connect(UserData, int messageId);
    QString getTouramentIds(int messageId);
    QString subscribeWebTournament(QString name);
    QString subscribeBroadcastChessGame(QString name);
    QString getBroadcastChessGame(int,QString name);
    QString getWebTournament(int messageId, QString name);
    QVariantList tournamentNamesFromJSON(QString data);


    namespace Helpers{
        const QString model_webTournament = "model:webTournamentRedisAR:";
        const QString model_broadcastChessGame = "model:BroadcastChessGameRedisAR:";
        int findDataStart(QString);
        QString getData(QString);
        QString getHeader(QString);
        QString buildData(QString name, QVariantList args);
        QString mapToString(QVariantMap);
    }
};

#endif // CHESS24MESSAGES_H
