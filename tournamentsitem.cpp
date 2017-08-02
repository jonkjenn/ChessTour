#include "tournamentsitem.h"

TournamentsItem::TournamentsItem(TournamentsItem *parentItem, int row):
    parent(parentItem),row(row)
{

}

TournamentsItem *TournamentsItem::child(int pos) const
{
    return childItems[pos];
}

int TournamentsItem::childCount() const
{
    return childItems.size();
}

TournamentsItem *TournamentsItem::parentItem() const
{
    return this->parent;
}
