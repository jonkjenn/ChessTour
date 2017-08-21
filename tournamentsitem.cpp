#include "tournamentsitem.h"

TournamentsItem::TournamentsItem(TournamentsItem *parentItem, ItemType type):
    tourType(type),parent(parentItem)
{
}

TournamentsItem *TournamentsItem::parentItem()
{
    return this->parent;
}
