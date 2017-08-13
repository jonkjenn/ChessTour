#ifndef TOURNAMENTSITEM_H
#define TOURNAMENTSITEM_H

#include <QObject>
#include <QVector>
#include <QVariant>
#include <QNetworkReply>

class TournamentsItem
{
public:
    enum ItemType{
        Root,
        Tournament,
        Round,
        Match
    };

    TournamentsItem(TournamentsItem* parentItem = 0,ItemType type=Root);
    TournamentsItem *child(int position) const;
    int childCount() const;
    virtual int columnCount() const{
        return 0;
    }
    virtual QVariant data(int role) const{
        return QVariant();
    }
    TournamentsItem* parentItem() const;
    virtual ~TournamentsItem(){}
    void addChild(TournamentsItem*);
    ItemType tourType;
private:
    QVector<TournamentsItem*> childItems;
    TournamentsItem *parent = nullptr;
};

#endif // TOURNAMENTSITEM_H
