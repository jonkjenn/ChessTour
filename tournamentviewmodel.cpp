#include "tournamentviewmodel.h"
#include "tournamentsmodel.h"
#include "tournament.h"

TournamentViewModel::TournamentViewModel(QObject *parent, int tournamentId)
    :QSortFilterProxyModel(parent),tournamentId(tournamentId)
{

}

QHash<int, QByteArray> TournamentViewModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles[Tournament::TournamentRoles::NameRole] = "name";
    return roles;
}

/*QModelIndex TournamentViewModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if(proxyIndex.internalPointer() == nullptr){
        TournamentsModel *tms = static_cast<TournamentsModel*>(sourceModel());
        QModelIndex qm = createIndex(proxyIndex.column(),proxyIndex.row(),tms->rootItem);
        //QVariant var = sourceModel()->data(qm);
        return qm;
    }
    //TournamentsItem *tm = static_cast<TournamentsItem *>(proxyIndex.internalPointer());
}

QModelIndex TournamentViewModel::mapFromSource(const QModelIndex &sourceIndex) const
{
}*/
