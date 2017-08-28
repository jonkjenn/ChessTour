#ifndef TOURNAMENTSITEM_H
#define TOURNAMENTSITEM_H

class TournamentsItem
{
public:
    enum class ItemType{
        Root,
        Tournament,
        Round,
        Match
    };

    TournamentsItem(ItemType type);
    TournamentsItem(ItemType type, int tourId);
    TournamentsItem(ItemType type, int tourId, int roundId);
    virtual ~TournamentsItem() = default;

    const int tourId = -1;
    const int roundId = -1;
    const ItemType tourType;
};



#endif // TOURNAMENTSITEM_H
