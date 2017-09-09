#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <QObject>
#include "internalmessages.h"
#include "chess24manager.h"
#include "chess24sqlhandler.h"

class DataManager:public QObject
{
    Q_OBJECT
public:
    DataManager(QObject *parent, const Chess24Manager &c24Manager, Chess24SqlHandler &sqlHandler);
public slots:
    void onCurrentTournamentChanged(InternalMessages::TournamentChangedData data);
    void onRequestUpdateTournament(InternalMessages::TournamentChangedData data);
    void onRequestUpdateTournamentList();
signals:
    void tournamentLoaded(InternalMessages::TournamentChangedData data);
    void tournamentListChanged();
private:
    void subscribeTournament(QString name);
    void getTournament(InternalMessages::TournamentChangedData data);
    void getTournamentList();
    const Chess24Manager &c24Manager;
    Chess24SqlHandler &sqlHandler;
};

#endif // DATAMANAGER_H
