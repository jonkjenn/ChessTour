#include "round.h"

Round::Round(TournamentsItem *parent):TournamentsItem(parent,ItemType::Round)
{

}

QVariant Round::data(int role) const
{
    switch(static_cast<RoundRoles>(role)){
    case RoundRoles::NumberRole:
        return number;
    case RoundRoles::DateRole:
        return date;
    case RoundRoles::DescriptionRole:
        return description;
    }

    return QVariant();
}


TournamentsItem *Round::child(int position) const
{
    return static_cast<TournamentsItem*>(matches.at(position));
}

int Round::childCount() const
{
    return matches.size();
}

bool Round::addChild(Match *match)
{
    matches.append(match);
}
