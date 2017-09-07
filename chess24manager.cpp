#include "chess24manager.h"
#include "chess24websocket.h"
#include "chess24messages.h"
#include "chess24sqlhandler.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <optional>

Chess24Manager::Chess24Manager(QObject *parent, Chess24Websocket &c24ws, Chess24SqlHandler &sqlHandler,
                               TournamentsSqlModel &tsm, RoundsSqlModel &rsm, LiveMatchSqlModel &lsm,
                               TokenContainer &tournamentToken, TokenContainer &roundToken
                               ):
    QObject(parent),
    tournamentToken(tournamentToken), tournamentListToken(roundToken),
    c24ws(c24ws), sqlHandler(sqlHandler),tsm(tsm),rsm(rsm),lsm(lsm)
{
    connect(&tournamentListToken,&TokenContainer::notifyTokenAvailableChanged,[this](bool val){
        emit canRefreshTournamentListChanged(val);
    });
    connect(&tournamentToken,&TokenContainer::notifyTokenAvailableChanged,[this](bool val){
        emit canRefreshTournamentChanged(val);
    });
}

WSRequest *Chess24Manager::sendMessage(std::function<QString(int)> msgFunc){

    int id = c24ws.increaseAndGetMessageId();
    return c24ws.sendMessage(msgFunc(id),id);
}

WSRequest *Chess24Manager::sendMessage(std::function<QString(int,QString)> msgFunc,QString msgData){

    int id = c24ws.increaseAndGetMessageId();
    return c24ws.sendMessage(msgFunc(id,msgData),id);
}

void Chess24Manager::refreshTournamentList(){
    if(tournamentListToken.getToken()){
        getTournamentList();
    }
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

void Chess24Manager::getTournamentList(){
    WSRequest *req = sendMessage(Chess24Messages::getTouramentIds);
    connect(req,&WSRequest::finished,[this](QString data){
        qDebug() << "Got tournament list";
        QVariantList tournaments = Chess24Messages::tournamentNamesFromJSON(data);
        sqlHandler.addTournaments(tournaments);
        tsm.forceRefresh();
    });
}

void Chess24Manager::subscribeTournament(QString name)
{
    c24ws.sendMessage(Chess24Messages::subscribeWebTournament(name));
}

void Chess24Manager::getTournament(InternalMessages::TournamentChangedData data){
    WSRequest *req = sendMessage(Chess24Messages::getWebTournament,data.name);
    connect(req,&WSRequest::finished,[this,data](QString jsonData){
        QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
        if(!doc.isArray()){
            return;
        }
        QJsonArray arr =  doc.array();
        if(arr.size() <= 0){
            return;
        }
        if(!arr.at(0).isObject()){
            return;
        }
        //sqlHandler.updateTournamentDetails(arr.at(0).toObject());
        QVariantMap transform = Chess24Messages::transformWebTournament(arr.at(0).toObject().toVariantMap());
        sqlHandler.updateTournament(data.name,transform);
        emit tournamentLoaded(data);
    });
}

void Chess24Manager::onCurrentTournamentChanged(InternalMessages::TournamentChangedData data){
    //QString name = tsm.data(row,"Name").toString();


    qDebug() << "Subscribe to " << data.name;

    QDateTime lastUpdated = sqlHandler.lastUpdated(data.pk);

    if(lastUpdated.isValid()){
        //Don't automatically update tournaments which already has been updated the last 10 minutes
        if(lastUpdated.msecsTo(QDateTime::currentDateTimeUtc())<1000*60*10){
            subscribeTournament(data.name);
            emit tournamentLoaded(data);
            return;
        }
    }

    getTournament(data);
}

void Chess24Manager::refreshTournament(int row)
{
    if(tournamentToken.getToken()){
        getTournament(tsm.getTournamentData(row));
    }
}

bool Chess24Manager::canRefreshTournamentList() const
{
    return tournamentListToken.tokenAvailable();
}

bool Chess24Manager::canRefreshTournament() const
{
    return tournamentToken.tokenAvailable();
}
