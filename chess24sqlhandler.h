#ifndef CHESS24SQLHANDLER_H
#define CHESS24SQLHANDLER_H

#include <QObject>
#include <QSqlDatabase>

#include "tournamentssqlmodel.h"

class QJsonObject;
class Chess24Manager;

class Chess24SqlHandler:public QObject
{
    Q_OBJECT
public:
    Chess24SqlHandler(QObject *parent, QSqlDatabase database, TournamentsSqlModel *tsm, Chess24Manager &c24Manager);
    void updateTournamentDetails(QJsonObject json);
    bool addTournaments(QVariantList names);

    void updatePlayers(const QJsonObject &players);
    bool updateMatches(QJsonObject matches, int roundPk);
    bool updateRounds(QJsonObject rounds, int tournamentPk);
public slots:
    void onCurrentTournamentChanged(int row);

private:
    QSqlDatabase database;
    TournamentsSqlModel *tsm = nullptr;
    Chess24Manager &c24Manager;
};

#endif // CHESS24SQLHANDLER_H
