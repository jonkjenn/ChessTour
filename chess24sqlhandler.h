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
    void updateTournamentDetails(QJsonObject json);
    Chess24SqlHandler(QObject *parent);
    bool addTournaments(QVariantList names) ;
    void onWebTournamentRedisAR(WebTournamentRedisAR msg);
    QDateTime lastUpdated(int row);

    bool updateTournament(QString name, QVariantMap map, int currentTourPk, int currentRoundPk);
    void insertUpdatePlayers(QVariantList Name, QVariantList FideId);
    std::optional<int> getTournamentPk(QString name);
    QVariantMap insertUpdateRounds(const QVariantList &Number, const QVariantList &rounds, const int tournamentPk,bool returnChanges=false);
    std::optional<int> getRoundPk(QString number, int tournamentId);
    QVariantList insertUpdateMatches(const QVariantList &Number, const QVariantList &GameNumber, const QVariantList &games, const int roundPk, bool returnChanges=false);
    std::optional<int> getGamePk(QString number, QString gameNumber, int roundId);
    QVariantList getRoundPk(QVariantList number, int tournamentId);
    QString updateQueryFromMap(const QVariantMap &map);
    QVariantMap updateTournament(QString name, QVariantMap map, bool returnChanges = false);
    void insertLists(const QString table, const QVector<QVariantList> &lists, const QVector<QString> &names);
    QVariantList getColumnList(QString table, QString column, int listSize, QVariantMap &whereLists, QVariantMap &whereValues);
    QVariantList getMatchPks(QVariantList Number, QVariantList GameNumber, int roundPk);
    void updateTable(QString table, const QVariantMap &map, QVariantMap &whereMap);
private:
    TournamentsSqlModel &tsm;
    RoundsSqlModel &rsm;
    LiveMatchSqlModel &lsm;

    QSqlDatabase database = QSqlDatabase::database();

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
