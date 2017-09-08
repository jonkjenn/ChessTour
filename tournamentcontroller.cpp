#include "tournamentcontroller.h"
#include "chess24messages.h"
#include "wsrequest.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

TournamentController::TournamentController(QObject *parent,
                                           const Chess24Manager &manager,
                                           TournamentsSqlModel &tsm,
                                           TokenContainer &tournamentToken,
                                           TokenContainer &roundToken,
                                           Chess24SqlHandler &sqlHandler
                                           )
    :QObject(parent),
    tournamentToken(tournamentToken), tournamentListToken(roundToken),
    tsm(tsm), manager(manager), sqlHandler(sqlHandler)
{
    connect(&tournamentListToken,&TokenContainer::notifyTokenAvailableChanged,[this](bool val){
        emit canRefreshTournamentListChanged(val);
    });
    connect(&tournamentToken,&TokenContainer::notifyTokenAvailableChanged,[this](bool val){
        emit canRefreshTournamentChanged(val);
    });

}

void TournamentController::getTournament(InternalMessages::TournamentChangedData data){
    WSRequest *req = manager.sendMessage(Chess24Messages::getWebTournament,data.name);
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

        QVariantMap transform = Chess24Messages::transformWebTournament(arr.at(0).toObject().toVariantMap());
        sqlHandler.updateTournament(data.name,transform);
        emit tournamentLoaded(data);
    });
}

void TournamentController::getTournamentList(){
    WSRequest *req = manager.sendMessage(Chess24Messages::getTouramentIds);
    connect(req,&WSRequest::finished,[this](QString data){
        qDebug() << "Got tournament list";
        QVariantList tournaments = Chess24Messages::tournamentNamesFromJSON(data);
        sqlHandler.addTournaments(tournaments);
        tsm.forceRefresh();
    });
}


void TournamentController::refreshTournament(int row)
{
    if(tournamentToken.getToken()){
        getTournament(tsm.getTournamentData(row));
    }
}

bool TournamentController::canRefreshTournamentList() const
{
    return tournamentListToken.tokenAvailable();
}

bool TournamentController::canRefreshTournament() const
{
    return tournamentToken.tokenAvailable();
}

void TournamentController::refreshTournamentList(){
    if(tournamentListToken.getToken()){
        getTournamentList();
    }
}


void TournamentController::onCurrentTournamentChanged(InternalMessages::TournamentChangedData data){

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

void TournamentController::subscribeTournament(QString name)
{
    manager.sendMessage(Chess24Messages::subscribeWebTournament(name));
}
