#ifndef ROOTITEM_H
#define ROOTITEM_H

#include <QMap>

#include "tournamentsitem.h"
#include "tournament.h"


class RootItem:public TournamentsItem
{
public:
    RootItem(TournamentsItem *parent);

    // TournamentsItem interface
    int columnCount() const override;
    QVariant data(int role) const override;
    TournamentsItem *child(int position) const override;
    int childCount() const override;
    bool addChild(TournamentsItem *) override;
    int getPosition(QString key) const;
    bool addChildren(QVector<Tournament *> newChildren);
    int position(TournamentsItem *) override;
    const Tournament* tournamentChild(int position);
    Tournament *item(QString name);
    int position(QString name);
private:
    //QMap<QString,Tournament*> children;
    QVector<Tournament*> children;

    // TournamentsItem interface
public:
    bool setData(int role, const QVariant &value) override;
};

#endif // ROOTITEM_H
