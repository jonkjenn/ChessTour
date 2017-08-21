#include "tournamentsitem.h"

TournamentsItem::TournamentsItem(TournamentsItem *parentItem, ItemType type):
    tourType(type),parent(parentItem)
{
}

{
    return this->parent;
}
