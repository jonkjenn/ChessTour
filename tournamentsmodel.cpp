#include "tournamentsmodel.h"
#include <QObject>
#include <QtNetwork>
#include <iostream>

#include "tournament.h"
#include "tournamentsitem.h"

TournamentsModel::TournamentsModel(QObject *parent):QAbstractItemModel(parent)
{
    rootItem = new TournamentsItem(0,Tournament::ItemType::Root);
    TournamentsItem *tm1 = new Tournament(rootItem,"Test tournament1");
    rootItem->addChild(tm1);
}

TournamentsModel::~TournamentsModel()
{
    delete(rootItem);
}

QModelIndex TournamentsModel::index(int row, int column, const QModelIndex &parent) const
{
    //Return invalid modelindex
    if(!hasIndex(row,column,parent)){
        return QModelIndex();
    }

    TournamentsItem *parentItem;
    if(!parent.isValid()){
        parentItem = rootItem;
    }else{
        parentItem = static_cast<TournamentsItem*>(parent.internalPointer());
    }

    TournamentsItem *childItem = parentItem->child(row);
    if(childItem){
        return createIndex(row,column,childItem);
    }else{
        return QModelIndex();//Return invalid index
    }
}

QModelIndex TournamentsModel::parent(const QModelIndex &child) const
{
    if(!child.isValid()){
        return QModelIndex();
    }

    TournamentsItem *childItem = static_cast<TournamentsItem*>(child.internalPointer());
    TournamentsItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem){
        return QModelIndex();
    }

    return createIndex(0,0,parentItem);
}

int TournamentsModel::rowCount(const QModelIndex &parent) const
{
    TournamentsItem *parentItem;

    if(!parent.isValid()){
        parentItem = rootItem;
    }else{
        parentItem = static_cast<TournamentsItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int TournamentsModel::columnCount(const QModelIndex &parent) const
{
    TournamentsItem *tm;
    if(parent.isValid()){
        tm = static_cast<TournamentsItem*>(parent.internalPointer());
    }else{
        tm = rootItem;
    }

    if(tm->childCount()==0){
        return 0;
    }else{
        return tm->child(0)->columnCount();
    }
}

QVariant TournamentsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        return QVariant();
    }

    TournamentsItem *item = static_cast<TournamentsItem*>(index.internalPointer());

    if(item->tourType == TournamentsItem::ItemType::Tournament){
        Tournament *t = static_cast<Tournament*>(item);
        return t->data(role);
    }

    return item->data(role);
}

bool TournamentsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
}

QVariant TournamentsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
}

bool TournamentsModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
}

bool TournamentsModel::insertColumns(int column, int count, const QModelIndex &parent)
{
}

bool TournamentsModel::removeColumns(int column, int count, const QModelIndex &parent)
{
}

Qt::ItemFlags TournamentsModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()){
        return 0;
    }

    return QAbstractItemModel::flags(index);
}

bool TournamentsModel::hasChildren(const QModelIndex &parent) const
{
    if(parent.isValid()){
        TournamentsItem *tm = static_cast<TournamentsItem *>(parent.internalPointer());
        if(tm->childCount()>0){
            return true;
        }
    }

    return false;
}


