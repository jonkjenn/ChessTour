#include "datamanager.h"
#include "wsrequest.h"
#include "chess24messages.h"
#include <QJsonDocument>
#include <QJsonArray>

DataManager::DataManager(QObject *parent, const Chess24Manager &c24Manager, Chess24SqlHandler &sqlHandler)
    :QObject(parent),
      c24Manager(c24Manager), sqlHandler(sqlHandler)
{

}

void DataManager::getTournament(InternalMessages::TournamentChangedData data){
    WSRequest *req = c24Manager.sendMessage(Chess24Messages::getWebTournament,data.name);
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

void DataManager::getTournamentList(){
    WSRequest *req = c24Manager.sendMessage(Chess24Messages::getTouramentIds);
    connect(req,&WSRequest::finished,[this](QString data){
        qDebug() << "Got tournament list";
        QVariantList tournaments = Chess24Messages::tournamentNamesFromJSON(data);
        sqlHandler.addTournaments(tournaments);
        emit tournamentListChanged();
    });
}


void DataManager::subscribeTournament(QString name)
{
    c24Manager.sendMessage(Chess24Messages::subscribeWebTournament(name));
}

void DataManager::onCurrentTournamentChanged(InternalMessages::TournamentChangedData data){

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
    subscribeTournament(data.name);
}

void DataManager::onRequestUpdateTournament(InternalMessages::TournamentChangedData data)
{
    getTournament(data);
}

void DataManager::onRequestUpdateTournamentList()
{
    getTournamentList();
}
