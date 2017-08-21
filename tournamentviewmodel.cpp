#include "tournamentviewmodel.h"
#include "tournamentsmodel.h"
#include "tournament.h"

TournamentViewModel::TournamentViewModel(QObject *parent, bool onlySubscribed)
    :QSortFilterProxyModel(parent),onlySubscribed(onlySubscribed)
{
    setSortRole(static_cast<int>(Tournament::TournamentRoles::OriginalOrderRole));
    setDynamicSortFilter(true);
    //sort(0);
}

QHash<int, QByteArray> TournamentViewModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles[static_cast<int>(Tournament::TournamentRoles::NameRole)] = "name";
    roles[static_cast<int>(Tournament::TournamentRoles::OriginalOrderRole)] = "originalOrder";
    return roles;
}

bool TournamentViewModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    //Only show the tournaments that are subscribed to
    if(onlySubscribed){
        TournamentsModel *tms = static_cast<TournamentsModel*>(sourceModel());
        //If the parent item is the root item, we know that this is a tournament
        if(static_cast<TournamentsItem *>(source_parent.internalPointer()) == tms->rootItem){
            TournamentsItem *tm = static_cast<TournamentsItem *>(source_parent.internalPointer());
            if((static_cast<Tournament*>(tm->child(source_row)))->subscribed){
                return true;
            }
        }
        return false;
    }else{
        return true;
    }
}


bool TournamentViewModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    return source_left.data(sortRole()) < source_right.data(sortRole());
}
