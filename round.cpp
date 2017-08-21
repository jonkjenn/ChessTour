#include "round.h"

Round::Round(TournamentsItem *parent):TournamentsItem(parent,ItemType::Round)
{

}

QVariant Round::data(int role) const
{
    switch(role){
    case RoundRoles::NumberRole:
        return number;
    case RoundRoles::DateRole:
        return date;
    }

    return QVariant();
}


TournamentsItem *Round::child(int position) const
{
}

int Round::childCount() const
{
}

bool Round::addChild(TournamentsItem *)
{
}
