#ifndef TOURNAMENTSITEM_H
#define TOURNAMENTSITEM_H

#include <QObject>
#include <QVector>
#include <QVariant>
#include <QNetworkReply>

class TournamentsItem
{
public:
    enum class ItemType{
        Root,
        Tournament,
        Round,
        Match
    };

    TournamentsItem(TournamentsItem* parentItem ,ItemType type);
    virtual ~TournamentsItem() = default;

    virtual TournamentsItem *child(int position) const = 0;
    virtual int childCount() const = 0;
    virtual int columnCount() const = 0;
    virtual QVariant data(int role) const = 0;
    virtual bool addChild(TournamentsItem*) = 0;
    virtual int position(TournamentsItem*) = 0;
    TournamentsItem* parentItem();
    const ItemType tourType;
protected:
    TournamentsItem *parent = nullptr;
};



#endif // TOURNAMENTSITEM_H
