#ifndef CHESS24SQLHANDLER_H
#define CHESS24SQLHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <optional>

#include "tournamentssqlmodel.h"
#include "roundssqlmodel.h"
#include "livematchsqlmodel.h"
#include "chess24messageparser.h"

class QJsonObject;
class Chess24Manager;

class Chess24SqlHandler:public QObject
{
    Q_OBJECT
public:
    Chess24SqlHandler(QObject *parent, QSqlDatabase database, TournamentsSqlModel &tsm, RoundsSqlModel &rsm,LiveMatchSqlModel &lsm);
    void updateTournamentDetails(QJsonObject json);
    bool addTournaments(QVariantList names) ;
    void onWebTournamentRedisAR(WebTournamentRedisAR msg);
    QDateTime lastUpdated(int row);

private:
    QSqlDatabase database;
    TournamentsSqlModel &tsm;
    RoundsSqlModel &rsm;
    LiveMatchSqlModel &lsm;

    void updateMatch(int matchNumber, int gameNumber, QJsonObject data);
    void updateLiveGame(int matchNumber, int gameNumber, QJsonObject game, int roundPk);
    void setData(QMap<QString, QVariant> &updates, int rowId, QString column, QVariant value);
    void updateLiveRounds(QJsonObject rounds);
    void updateLiveMatches(QJsonObject matches, int roundPk);
    void parseSubtree(QMap<QString, QVariant> &updates, const QVariantMap &valid, const QJsonObject &root);
    void updateDbGameFromUpdates(const QMap<QString, QVariant> &updates, int matchNumber, int gameNumber, int roundPk);
    void updateLiveGameFromUpdates(const QMap<QString, QVariant> &updates, int rowId);
    void updatePlayers( QJsonObject &players) ;
    bool updateMatches(QJsonObject matches, int roundPk) ;
    bool updateRounds(QJsonObject rounds, int tournamentPk) ;
    std::optional<QVariant> getString(QJsonObject source, QString key);
    std::optional<int> getInt(QJsonObject source, QString key);
};

#endif // CHESS24SQLHANDLER_H
