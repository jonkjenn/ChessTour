#include "chess24manager.h"
#include "chess24websocket.h"
#include "chess24messages.h"

#include "tournament.h"
#include "tournamentsitem.h"

#include <QJsonDocument>

Chess24Manager::Chess24Manager(QObject *parent, Chess24Websocket &c24ws):
    QObject(parent),
    c24ws(c24ws)
{

}

WSRequest *Chess24Manager::sendMessage(std::function<QString(int)> msgFunc){

    int id = c24ws.messageId();
    return c24ws.sendMessage(msgFunc(id),id);
}

WSRequest *Chess24Manager::sendMessage(std::function<QString(int,QString)> msgFunc,QString msgData){

    int id = c24ws.messageId();
    return c24ws.sendMessage(msgFunc(id,msgData),id);
}

//Caller should be subscribed to gotTournaments for the result
void Chess24Manager::getTournaments(){
    WSRequest *req = sendMessage(Chess24Messages::getTouramentIds);
    connect(req,&WSRequest::finished,[this](QString data){
        qDebug() << "Got tournament list";
        QVariantList tournaments = Chess24Messages::tournamentNamesFromJSON(data);
        emit gotTournaments(tournaments);
    });
}

void Chess24Manager::subscribeTournament(QString name)
{
    c24ws.sendMessage(Chess24Messages::subscribeWebTournament(name));
}

void Chess24Manager::getTournament(QString name){
    WSRequest *req = sendMessage(Chess24Messages::getWebTournament,name);
    connect(req,&WSRequest::finished,[this](QString data){
        emit gotTournamentDetails(QJsonDocument::fromJson(data.toUtf8()).object());
    });
}
