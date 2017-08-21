#include "tournament.h"

Tournament::Tournament(TournamentsItem *parent,QString name,int originalOrder):
    TournamentsItem(parent,ItemType::Tournament),name(name), originalOrder(originalOrder)
{
}

int Tournament::columnCount() const
{
    return 1;
}

QVariant Tournament::data(int role) const
{
    switch(static_cast<TournamentRoles>(role)){
        case TournamentRoles::NameRole:
            return this->name;
    case TournamentRoles::OriginalOrderRole:
        return this->originalOrder;
    }
}


TournamentsItem *Tournament::child(int position) const
{
    return rounds.at(position);
}

int Tournament::childCount() const
{
    return rounds.size();
}

bool Tournament::addChild(TournamentsItem *)
{
}


int Tournament::position(TournamentsItem *item)
{
    Round *r = static_cast<Round*>(item);
    return rounds.indexOf(r);
}
