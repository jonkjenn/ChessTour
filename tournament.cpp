#include "tournament.h"

Tournament::Tournament(TournamentsItem *parent,QString name):TournamentsItem(parent,ItemType::Tournament),name(name)
{
}

int Tournament::columnCount() const
{
    return 1;
}

QVariant Tournament::data(int role) const
{
    switch(role){
        case TournamentRoles::NameRole:
            return this->name;
    }
}
