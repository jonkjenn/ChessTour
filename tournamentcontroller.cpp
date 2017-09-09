#include "tournamentcontroller.h"

#include "chess24messages.h"
#include "wsrequest.h"
#include "sqlhelper.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

TournamentController::TournamentController(QObject *parent,
                                           TournamentsSqlModel &tsm,
                                           TokenContainer &tournamentToken,
                                           TokenContainer &roundToken
                                           )
    :QObject(parent),
    tournamentToken(tournamentToken), tournamentListToken(roundToken),
    tsm(tsm)
{
    connect(&tournamentListToken,&TokenContainer::notifyTokenAvailableChanged,[this](bool val){
        emit canRefreshTournamentListChanged(val);
    });
    connect(&tournamentToken,&TokenContainer::notifyTokenAvailableChanged,[this](bool val){
        emit canRefreshTournamentChanged(val);
    });

}

void TournamentController::refreshTournament(int row)
{
    if(tournamentToken.getToken()){
        emit requestUpdateTournament(tsm.getTournamentData(row));
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
        emit requestUpdateTournamentList();
    }
}

