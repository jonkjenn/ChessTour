#include "chess24manager.h"
#include "chess24websocket.h"
#include "chess24messages.h"
#include "chess24sqlhandler.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <optional>

Chess24Manager::Chess24Manager(QObject *parent, Chess24Websocket &c24ws, Chess24SqlHandler &sqlHandler,
                               TournamentsSqlModel &tsm, RoundsSqlModel &rsm, LiveMatchSqlModel &lsm)
    :QObject(parent),
    c24ws(c24ws), sqlHandler(sqlHandler),tsm(tsm),rsm(rsm),lsm(lsm)
{
}

WSRequest *Chess24Manager::sendMessage(std::function<QString(int)> msgFunc) const {

    int id = c24ws.increaseAndGetMessageId();
    return c24ws.sendMessage(msgFunc(id),id);
}

WSRequest *Chess24Manager::sendMessage(std::function<QString(int,QString)> msgFunc,QString msgData) const{

    int id = c24ws.increaseAndGetMessageId();
    return c24ws.sendMessage(msgFunc(id,msgData),id);
}

void Chess24Manager::sendMessage(QString msg) const
{
    c24ws.sendMessage(msg);
}


void Chess24Manager::onWebTournamentRedisAR(WebTournamentRedisAR msg){
    QVariantMap map = msg.args.toVariantMap();
    if(map.keys().contains("diffs")){
        QVariantMap transform = Chess24Messages::transformWebTournament(map.value("diffs").toMap());

        QVariantMap changes = sqlHandler.updateTournament(msg.tournament,transform,true);
        if(rsm.currentTournamentPk() == changes.value("TournamentPk").toInt()){
            lsm.possibleUpdates(changes.value("match").toList());
        }
        //sqlHandler.getPksColumns(msg.tournament,transform,rsm.currentPK(),lsm.currentPk());
    }
}


