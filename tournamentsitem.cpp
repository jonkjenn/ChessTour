#include "tournamentsitem.h"

TournamentsItem::TournamentsItem(TournamentsItem::ItemType type)
{

}

TournamentsItem::TournamentsItem(TournamentsItem::ItemType type, int tourId):tourId(tourId)
{

}

TournamentsItem::TournamentsItem(TournamentsItem::ItemType type, int tourId, int roundId):tourId(tourId),roundId(roundId)
{

}
