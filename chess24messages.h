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
    QString heartBeat();

    void insertFlat(QVariantMap &out, const QVariantMap &root,const QString &name, const QString &column);
    void insertNest(QVariantMap &out, const QVariantMap &root,const QVector<QString> &name, const QString &column);
    QVariantMap transformWebTournament(const QVariantMap &root);
    QVariantMap validWebTournamentGame();
    void parseSubtree(QMap<QString,QVariant> &updates,const QVariantMap &valid,const QVariantMap &root);

    //QVariantMap validWebTournamentJson();

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
