#include "round.h"

Round::Round(TournamentsItem *parent,int number):
    TournamentsItem(parent,ItemType::Round),
    number(number)
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

int Round::columnCount() const
{
}

bool Round::addChild(TournamentsItem *match)
{
    if(match->tourType != TournamentsItem::ItemType::Match){return false;}
    matches.append(static_cast<Match*>(match));
    return true;
}

int Round::position(TournamentsItem *item)
{
    if(item->tourType != TournamentsItem::ItemType::Match){return -1;}
    return matches.indexOf(static_cast<Match *>(item));
}

int Round::position(int id)
{
    for(int i=0;i<matches.size();++i){
        if(matches.at(i)->id == id){
            return i;
        }
    }
    return -1;
}

bool Round::setData(int role, const QVariant &value)
{
    switch (static_cast<RoundRoles>(role)) {
    case RoundRoles::DateRole:
        if(date == value.toString()){return false;}
        date = value.toString();
        return true;
    case RoundRoles::DescriptionRole:
        if(description == value.toString()){return false;}
        description = value.toString();
        return true;
    case RoundRoles::NumberRole:
        if(number == value.toInt()){return false;}
        number = value.toInt();
        return true;
    default:
        return false;
    }
}
