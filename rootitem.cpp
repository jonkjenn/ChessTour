#include "rootitem.h"

RootItem::RootItem(TournamentsItem *parent):
    TournamentsItem (parent,ItemType::Root)
{

}

const Tournament* RootItem::tournamentChild(int position){
    return children.at(position);
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

int RootItem::position(QString name)
{
    for(int i=0;i<children.size();++i){
        if(children.at(i)->name == name){
            return i;
        }
    }
    return -1;
}

Tournament *RootItem::item(QString name)
{
    auto pos = std::find_if(children.begin(),children.end(),[&name](const Tournament* t){return t->name == name;});
    if (pos != children.end()){
        return *pos;
    }
    else{
        return nullptr;
    }
}


bool RootItem::setData(int role, const QVariant &value)
{
    return false;
}
