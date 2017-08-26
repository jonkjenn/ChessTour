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

QModelIndex RoundViewModel::indexFromRow(int row,const QModelIndex &parent)
{
    QModelIndex i = index(row,0,parent);
    qDebug() << i.model();
    qDebug() << i.internalPointer();
    TournamentsItem *t = static_cast<TournamentsItem*>(i.internalPointer());
   return QModelIndex();
}

