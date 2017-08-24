#include "roundviewmodel.h"
#include "round.h"

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
