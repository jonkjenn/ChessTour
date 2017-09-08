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
#include "chess24messageparser.h"
#include "internalmessages.h"

class Chess24Websocket;
class Chess24SqlHandler;
class WSRequest;

class Chess24Manager: public QObject
{
    Q_OBJECT
public:
    Chess24Manager(QObject *parent, Chess24Websocket &c24ws, Chess24SqlHandler &sqlHandler,
                   TournamentsSqlModel &tsm, RoundsSqlModel &rsm, LiveMatchSqlModel &lsm);
    void subscribeTournament(QString name);
    void onWebTournamentRedisAR(WebTournamentRedisAR msg);

    WSRequest *sendMessage(std::function<QString (int)> msg) const;
    WSRequest *sendMessage(std::function<QString (int, QString)> msgFunc, QString msgData) const;
    void sendMessage(QString msg) const;

private:
    Chess24Websocket &c24ws;
    Chess24SqlHandler &sqlHandler;
    TournamentsSqlModel &tsm;
    RoundsSqlModel &rsm;
    LiveMatchSqlModel &lsm;
    void getTournament(InternalMessages::TournamentChangedData data);
    void getTournamentList();
};



#endif // CHESS24MANAGER_H
