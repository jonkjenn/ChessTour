#include "match.h"

Match::Match(TournamentsItem *parent, const Round &round, const Tournament &tournament):
    TournamentsItem(parent,ItemType::Match),
    round(round),
    tournament(tournament)
{

}

int Match::columnCount() const
{
    return 1;
}

QVariant Match::data(int role) const
{
    switch(role){
    case MatchRoles::BlackRole:
        return black;
    case MatchRoles::WhiteRole:
        return white;
    case MatchRoles::PositionRole:
        return position;
    case MatchRoles::PriorityRole:
        return priority;
    case MatchRoles::ResultRole:
        return result;

    case MatchRoles::DeepChangeRole:
        return deep.change;
    case MatchRoles::DeepCurrentMoveRole:
        return deep.moveCurrent;
    case MatchRoles::DeepCurrentRole:
        return deep.current;
    case MatchRoles::DeepDepthRole:
        return deep.depth;
    case MatchRoles::DeepPreviousMoveRole:
        return deep.movePrevious;
    case MatchRoles::DeepPreviousRole:
        return deep.previous;

    case MatchRoles::QuickChangeRole:
        return quick.change;
    case MatchRoles::QuickCurrentMoveRole:
        return quick.moveCurrent;
    case MatchRoles::QuickCurrentRole:
        return quick.current;
    case MatchRoles::QuickDepthRole:
        return quick.depth;
    case MatchRoles::QuickPreviousMoveRole:
        return quick.movePrevious;
    case MatchRoles::QuickPreviousRole:
        return quick.previous;
    }

    return QVariant();
}


TournamentsItem *Match::child(int position) const
{
    return nullptr;
}

int Match::childCount() const
{
    return 0;
}

bool Match::addChild(TournamentsItem *)
{
    return false;
}
