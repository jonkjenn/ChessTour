#ifndef CHESS24MANAGER_H
#define CHESS24MANAGER_H

#include <QObject>
#include <QVariant>
#include <QJsonObject>
#include <QTimer>

#include <functional>

#include "tournamentssqlmodel.h"
#include "roundssqlmodel.h"
#include "livematchsqlmodel.h"
#include "tokencontainer.h"
#include "chess24messageparser.h"
#include "internalmessages.h"

class Chess24Websocket;
class Chess24SqlHandler;
class WSRequest;

class Chess24Manager: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool canRefreshTournamentList READ canRefreshTournamentList NOTIFY canRefreshTournamentListChanged)
    Q_PROPERTY(bool canRefreshTournament READ canRefreshTournament NOTIFY canRefreshTournamentChanged)
public:
    Chess24Manager(QObject *parent, Chess24Websocket &c24ws, Chess24SqlHandler &sqlHandler,
                   TournamentsSqlModel &tsm, RoundsSqlModel &rsm, LiveMatchSqlModel &lsm,
                   TokenContainer &tournamentToken, TokenContainer &tournamentListToken
                   );
    void subscribeTournament(QString name);
    bool canRequestTournaments() const;
    TokenContainer &tournamentToken;
    TokenContainer &tournamentListToken;
    Q_INVOKABLE void refreshTournament(int rowId);
    Q_INVOKABLE void refreshTournamentList();

    bool canRefreshTournamentList() const;
    bool canRefreshTournament() const;
    void onWebTournamentRedisAR(WebTournamentRedisAR msg);

    void onCurrentTournamentChanged(InternalMessages::TournamentChangedData data);

signals:
    void canRefreshTournamentListChanged(bool canRefreshTournamentList);
    void canRefreshTournamentChanged(bool canRefreshTournament);
    void tournamentLoaded(InternalMessages::TournamentChangedData data);

private:
    Chess24Websocket &c24ws;
    Chess24SqlHandler &sqlHandler;
    WSRequest *sendMessage(std::function<QString (int)> msg);
    WSRequest *sendMessage(std::function<QString (int, QString)> msgFunc, QString msgData);
    TournamentsSqlModel &tsm;
    RoundsSqlModel &rsm;
    LiveMatchSqlModel &lsm;
    void getTournament(InternalMessages::TournamentChangedData data);
    void getTournamentList();
};



#endif // CHESS24MANAGER_H
