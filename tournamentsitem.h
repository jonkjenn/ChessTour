#ifndef TOURNAMENTSITEM_H
#define TOURNAMENTSITEM_H

#include <QVector>
#include <QVariant>

class TournamentsItem
{
public:
    TournamentsItem(TournamentsItem* parentItem = 0,int row=0);
    TournamentsItem *child(int pos) const;
    int childCount() const;
    virtual int columnCount() const{
        return 0;
    }
    virtual QVariant data(int column) const{
        return QVariant();
    }
    const int row;
    TournamentsItem* parentItem() const;
    virtual ~TournamentsItem(){}
private:
    QVector<TournamentsItem*> childItems;
    TournamentsItem *parent = nullptr;
};

#endif // TOURNAMENTSITEM_H
