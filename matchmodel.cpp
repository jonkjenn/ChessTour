#include "matchmodel.h"

MatchModel::MatchModel()
{

}

QHash<int,QByteArray> MatchModel::roleNames() const{
    QHash<int, QByteArray> roles;
    roles[ResultRole] = "result";
    roles[DeepChangeRole] = "deepchange";
    roles[DeepCurrentRole] = "deepcurrent";
    roles[DeepPreviousRole] = "deepprevious";
    roles[DeepDepthRole] = "deepdepth";
    roles[DeepMoveCurrentRole] = "deepmovecurrent";
    roles[DeepMovePreviousRole] = "deepmoveprevious";
    roles[QuickChangeRole] = "quickchange";
    roles[QuickCurrentRole] = "quickcurrent";
    roles[QuickPreviousRole] = "quickprevious";
    roles[QuickDepthRole] = "quickdepth";
    roles[QuickMoveCurrentRole] = "quickmovecurrent";
    roles[QuickMovePreviousRole] = "quickmoveprevious";
    roles[WhiteRole] = "white";
    roles[BlackRole] = "black";
    roles[PriorityRole] = "priority";
    roles[PositionRole] = "position";
    return roles;
}
