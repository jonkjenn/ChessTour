#include "roundviewmodel.h"
#include "round.h"
#include "tournamentsmodel.h"

RoundViewModel::RoundViewModel(QObject *parent):
    QSortFilterProxyModel (parent)
{

}

QHash<int, QByteArray> RoundViewModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles.insert(static_cast<int>(Round::RoundRoles::NumberRole),"number");
    roles.insert(static_cast<int>(Round::RoundRoles::DateRole),"date");
    roles.insert(static_cast<int>(Round::RoundRoles::DescriptionRole),"description");
    return roles;
}

QModelIndex RoundViewModel::createChildIndex(int parentRow, int parentParentRow)
{
    return sourceModel()->index(parentParentRow,0).child(parentRow,0);
}
