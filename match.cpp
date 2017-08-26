#include "match.h"
#include "tournament.h"

Match::Match(TournamentsItem *parent,int id):
    TournamentsItem(parent,ItemType::Match),
    id(id)
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
    {
        Tournament *t = static_cast<Tournament*>(parent->parentItem());
        if(t->players.contains(blackFide)){
            return t->players.value(blackFide);
        }
        return black;
    }
    case MatchRoles::WhiteRole:
    {
        Tournament *t = static_cast<Tournament*>(parent->parentItem());
        if(t->players.contains(whiteFide)){
            return t->players.value(whiteFide);
        }
        return white;
    }
    case MatchRoles::BlackFideRole:
    {
        return blackFide;
    }
    case MatchRoles::WhiteFideRole:
        return whiteFide;
    case MatchRoles::PositionRole:
        return gamePosition;
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

    case MatchRoles::FullGameRKeyRole:
        return fullGameRKey;

    case MatchRoles::IdRole:
        return id;
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

TournamentsItem *Match::child(int position) const
{
    return nullptr;
}

int Match::position(TournamentsItem *)
{
    return -1;
}

bool Match::setData(int role, const QVariant &value)
{
    MatchRoles r = static_cast<MatchRoles>(role);
    switch(r){
    case MatchRoles::BlackRole:
        if(value.toString() ==  black){return false;}
        black = value.toString();
        return true;
    case MatchRoles::WhiteRole:
        if(value.toString() ==  white){return false;}
        white = value.toString();
        return true;
    case MatchRoles::BlackFideRole:
        if(value.toInt() ==  blackFide){return false;}
        blackFide = value.toInt();
        return true;
    case MatchRoles::WhiteFideRole:
        if(value.toInt() ==  whiteFide){return false;}
        whiteFide = value.toInt();
        return true;
    case MatchRoles::PositionRole:
        if(value.toString() ==  gamePosition){return false;}
        gamePosition = value.toString();
        return true;
    case MatchRoles::PriorityRole:
        if(value.toInt() ==  priority){return false;}
        priority = value.toInt();
        return true;

    case MatchRoles::ResultWhiteRole:
        if(value.toString() ==  resultWhite){return false;}
        resultWhite = value.toString();
        return true;
    case MatchRoles::ResultBlackRole:
        if(value.toString() ==  resultBlack){return false;}
        resultBlack = value.toString();
        return true;

    case MatchRoles::DeepChangeRole:
        if(value.toFloat() ==  deep.change){return false;}
        deep.change = value.toFloat();
        return true;
    case MatchRoles::DeepCurrentMoveRole:
        if(value.toString() ==  deep.moveCurrent){return false;}
        deep.moveCurrent = value.toString();
        return true;
    case MatchRoles::DeepCurrentRole:
        if(value.toFloat() ==  deep.current){return false;}
        deep.current = value.toFloat();
        return true;
    case MatchRoles::DeepDepthRole:
        if(value.toInt() ==  deep.depth){return false;}
        deep.depth = value.toInt();
        return true;

    case MatchRoles::DeepPreviousMoveRole:
        if(value.toString() ==  deep.movePrevious){return false;}
        deep.movePrevious = value.toString();
        return true;
    case MatchRoles::DeepPreviousRole:
        if(value.toFloat() ==  deep.previous){return false;}
        deep.previous = value.toFloat();
        return true;

    case MatchRoles::QuickChangeRole:
        if(value.toFloat() ==  quick.change){return false;}
        quick.change = value.toFloat();
        return true;
    case MatchRoles::QuickCurrentMoveRole:
        if(value.toString() ==  quick.moveCurrent){return false;}
        quick.moveCurrent = value.toString();
        return true;
    case MatchRoles::QuickCurrentRole:
        if(value.toFloat() ==  quick.current){return false;}
        quick.current = value.toFloat();
        return true;
    case MatchRoles::QuickDepthRole:
        if(value.toInt() ==  quick.depth){return false;}
        quick.depth = value.toInt();
        return true;
    case MatchRoles::QuickPreviousMoveRole:
        if(value.toString() ==  quick.movePrevious){return false;}
        quick.movePrevious = value.toString();
        return true;
    case MatchRoles::QuickPreviousRole:
        if(value.toFloat() ==  quick.previous){return false;}
        quick.previous = value.toFloat();
        return true;

    case MatchRoles::FullGameRKeyRole:
        if(value.toString() ==  fullGameRKey){return false;}
        fullGameRKey = value.toString();
        return true;

    case MatchRoles::IdRole:
        if(value.toInt() ==  id){return false;}
        id = value.toInt();
        return true;

    }
}
