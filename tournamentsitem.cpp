#include "tournamentsitem.h"

TournamentsItem::TournamentsItem(TournamentsItem *parentItem, ItemType type):
    parent(parentItem),tourType(type)
{
}

TournamentsItem *TournamentsItem::child(int position) const
{
    return childItems[position];
}

int TournamentsItem::childCount() const
{
    return childItems.size();
}

TournamentsItem *TournamentsItem::parentItem() const
{
    return this->parent;
}

void TournamentsItem::addChild(TournamentsItem *child)
{
    childItems.append(child);
}
