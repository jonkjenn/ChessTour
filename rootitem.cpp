#include "rootitem.h"

RootItem::RootItem(TournamentsItem *parent):
    TournamentsItem (parent,ItemType::Root)
{

}


TournamentsItem *RootItem::child(int position) const
{
    return children.at(position);
}

int RootItem::childCount() const
{
    return children.size();
}

int RootItem::columnCount() const
{
    return 1;
}

QVariant RootItem::data(int role) const
{
    return QVariant();
}


bool RootItem::addChild(TournamentsItem *item)
{
    Tournament *t = static_cast<Tournament*>(item);

    for(auto c:children){
        if(c->name == t->name){
            return false;
        }
    }

    children.append(t);
    return true;
}


bool RootItem::addChildren(QVector<Tournament *> newChildren)
{

    for(auto c:newChildren){
        children.append(c);
    }
}


int RootItem::position(TournamentsItem *item)
{
    Tournament *t = static_cast<Tournament*>(item);
    return children.indexOf(t);
}
