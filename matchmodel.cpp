#include "matchmodel.h"

MatchModel::MatchModel()
{

}

QHash<int,QByteArray> MatchModel::roleNames() const{
    QHash<int, QByteArray> roles;
    roles[ResultRole] = "result";
    roles[DeepRole] = "deep";
    roles[QuickRole] = "quick";
    roles[WhiteRole] = "white";
    roles[BlackRole] = "black";
    roles[PriorityRole] = "priority";
    roles[PositionRole] = "position";
    return roles;
}
