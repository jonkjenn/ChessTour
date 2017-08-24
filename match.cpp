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
    switch(static_cast<MatchRoles>(role)){
    case MatchRoles::BlackRole:
        return black;
    case MatchRoles::WhiteRole:
        return white;
    case MatchRoles::BlackFideRole:
        return blackFide;
    case MatchRoles::WhiteFideRole:
        return whiteFide;
    case MatchRoles::PositionRole:
        return position;
    case MatchRoles::PriorityRole:
        return priority;
    case MatchRoles::ResultWhiteRole:
        return resultWhite;
    case MatchRoles::ResultBlackRole:
        return resultBlack;

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

int Match::childCount() const
{
    return 0;
}

bool Match::addChild(TournamentsItem *)
{
    return false;
}
