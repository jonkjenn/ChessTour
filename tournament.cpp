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
    return data(static_cast<TournamentRoles>(role));
}

QVariant Tournament::data(Tournament::TournamentRoles role) const
{
    switch(role){
        case TournamentRoles::NameRole:
            return this->name;
    case TournamentRoles::OriginalOrderRole:
        return this->originalOrder;
    case TournamentRoles::EventTypeRole:
        return this->eventType;
    case TournamentRoles::StatusRole:
        return this->status;
    case TournamentRoles::CurrentShownRoundRole:
        return this->currentShownRound;
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

bool Tournament::addChild(TournamentsItem *item)
{
    if(item->tourType != TournamentsItem::ItemType::Round){return false;}
    rounds.append(static_cast<Round*>(item));
    return true;
}

int Tournament::position(TournamentsItem *item)
{
    Round *r = static_cast<Round*>(item);
    return rounds.indexOf(r);
}

int Tournament::position(int id){
    for(int i=0;i<rounds.size();++i){
        if(rounds.at(i)->number == id){
            return i;
        }
    }
    return -1;
}


bool Tournament::setData(int role, const QVariant &value)
{
    switch(static_cast<TournamentRoles>(role)){
        case TournamentRoles::NameRole:
        if(name == value.toString()){return false;}
        name = value.toString();
        return true;
    case TournamentRoles::OriginalOrderRole:
        if(originalOrder == value.toInt()){return false;}
        originalOrder = value.toInt();
        return true;
    case TournamentRoles::EventTypeRole:
        if(eventType == value.toString()){return false;}
        eventType = value.toString();
        return true;
    case TournamentRoles::StatusRole:
        if(status == value.toString()){return false;}
        status = value.toString();
        return true;
    case TournamentRoles::CurrentShownRoundRole:
        if(currentShownRound == value.toInt()){return false;}
        currentShownRound = value.toInt();
        return true;

    default:
        return false;
    }
}
