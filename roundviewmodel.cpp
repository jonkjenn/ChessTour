#include "roundviewmodel.h"
#include "round.h"

RoundViewModel::RoundViewModel(QObject *parent):
    QSortFilterProxyModel (parent)
{

}


QHash<int, QByteArray> RoundViewModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    roles.insert(Round::RoundRoles::NumberRole,"number");
    roles.insert(Round::RoundRoles::DateRole,"date");
    roles.insert(Round::RoundRoles::DescriptionRole,"description");
}
