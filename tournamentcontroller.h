#ifndef TOURNAMENTCONTROLLER_H
#define TOURNAMENTCONTROLLER_H
#include "QObject"
#include "chess24manager.h"
#include "tournamentssqlmodel.h"
#include "tokencontainer.h"
#include "chess24sqlhandler.h"
#include "internalmessages.h"

class TournamentController:public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool canRefreshTournamentList READ canRefreshTournamentList NOTIFY canRefreshTournamentListChanged)
    Q_PROPERTY(bool canRefreshTournament READ canRefreshTournament NOTIFY canRefreshTournamentChanged)
public:
    TournamentController(QObject *parent, const Chess24Manager &manager, TournamentsSqlModel &tsm, TokenContainer &tournamentToken, TokenContainer &roundToken, Chess24SqlHandler &sqlHandler);

    bool canRequestTournaments() const;
    Q_INVOKABLE void refreshTournament(int rowId);
    Q_INVOKABLE void refreshTournamentList();

    bool canRefreshTournamentList() const;
    bool canRefreshTournament() const;
    void onCurrentTournamentChanged(InternalMessages::TournamentChangedData data);

signals:
    void canRefreshTournamentListChanged(bool canRefreshTournamentList);
    void canRefreshTournamentChanged(bool canRefreshTournament);
    void tournamentLoaded(InternalMessages::TournamentChangedData data);

private:
    TournamentsSqlModel &tsm;
    const Chess24Manager &manager;
    Chess24SqlHandler &sqlHandler;
    TokenContainer &tournamentToken;
    TokenContainer &tournamentListToken;

    void getTournament(InternalMessages::TournamentChangedData data);
    void getTournamentList();
    void subscribeTournament(QString name);
};

#endif // TOURNAMENTCONTROLLER_H
