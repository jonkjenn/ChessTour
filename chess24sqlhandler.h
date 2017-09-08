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
    Chess24SqlHandler(QObject *parent);
    bool addTournaments(QVariantList names) ;
    QVariantMap updateTournament(QString name, QVariantMap map, bool returnChanges = false) ;
    QDateTime lastUpdated(int pk) ;

private:
    QSqlDatabase database = QSqlDatabase::database();

    std::optional<int> getTournamentPk(QString name) ;
    QVariantList getRoundPk(QVariantList number, int tournamentId) ;
    std::optional<int> getRoundPk(QString number, int tournamentId) ;
    std::optional<int> getGamePk(QString number, QString gameNumber, int roundId) ;

    QVariantMap insertUpdateRounds(const QVariantList &Number, const QVariantMap &rounds, const int tournamentPk, bool returnChanges=false) ;
    QVariantList insertUpdateMatches(const QVariantList &Number, const QVariantList &GameNumber, const QVariantList &games, const int roundPk, bool returnChanges=false) ;

    QVariantList getMatchPks(QVariantList Number, QVariantList GameNumber, int roundPk) ;
    void insertUpdatePlayers(QVariantList Name, QVariantList FideId) ;
};

#endif // CHESS24SQLHANDLER_H
