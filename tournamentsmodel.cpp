#include "tournamentsmodel.h"
#include <QObject>
#include <QtNetwork>
#include <iostream>

#include "tournament.h"
#include "tournamentsitem.h"
#include "chess24manager.h"

TournamentsModel::TournamentsModel(QObject *parent, Chess24Manager &c24Manager):QAbstractItemModel(parent), c24Manager(c24Manager)
{
    rootItem = static_cast<TournamentsItem*>(&rootItemObject);
}

TournamentsModel::~TournamentsModel()
{
}

QModelIndex TournamentsModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row,column,parent)){
        return QModelIndex();
    }

    TournamentsItem *parentItem;
    if(!parent.isValid()){
        parentItem = rootItem;
    }else{
         parentItem = static_cast<TournamentsItem*>(parent.internalPointer())->child(parent.row());
    }

    return createIndex(row,column,parentItem);
}

QModelIndex TournamentsModel::parent(const QModelIndex &child) const
{
    if(!child.isValid()){
        return QModelIndex();
    }

    TournamentsItem *childItem = static_cast<TournamentsItem*>(child.internalPointer());
    TournamentsItem *parentItem = static_cast<TournamentsItem*>(childItem->parentItem());

    if(childItem == rootItem){
        return QModelIndex();
    }

    return createIndex(parentItem->position(childItem),0,parentItem);
}

int TournamentsModel::rowCount(const QModelIndex &parent) const
{
    TournamentsItem *parentItem;

    if(!parent.isValid()){
        parentItem = rootItem;
    }else{
        parentItem = static_cast<TournamentsItem*>(parent.internalPointer())->child(parent.row());
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

    return tm->columnCount();
}

QVariant TournamentsModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        return QVariant();
    }

    TournamentsItem *item = static_cast<TournamentsItem*>(index.internalPointer());

    /*if(item->tourType == TournamentsItem::ItemType::Root){
        return item->child(index.row())->data(role);
    }*/

    return item->child(index.row())->data(role);
}

bool TournamentsModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

bool TournamentsModel::addChild(const QModelIndex &parent, TournamentsItem *child)
{
    TournamentsItem *tm;

    if(!parent.isValid()){
        tm = rootItem;
    }else{
         tm = static_cast<TournamentsItem *>(parent.internalPointer());
    }

    if(tm == rootItem){
        beginInsertRows(parent,tm->childCount(),tm->childCount());//TODO The index will be wrong if QMap is used since it is automatically sorted, not sure of real effect
        tm->addChild(child);
        endInsertRows();
        return true;
    }

    return false;
}

bool TournamentsModel::addTournament(QString name,int originalOrder)
{
    Tournament *t = new Tournament(rootItem,name,originalOrder);
    return addChild(QModelIndex(),t);
}

//TODO implement this
bool TournamentsModel::addTournaments(QVariantList names)
{

    RootItem *root = static_cast<RootItem*>(rootItem);//TODO add addChildren to interface?

    //Create index to avoid duplicates
    QHash<QString,int> nameMap;
    for(int i=0;i<root->childCount();++i){
        nameMap.insert(root->tournamentChild(i)->data(Tournament::TournamentRoles::NameRole).toString(),i);
    }

    QVector<Tournament*> tournaments;
    for(int i=0;i<names.length();++i){
        //Avoid duplicates
        if(nameMap.contains(names.at(i).toString())){continue;}

        Tournament *t = new Tournament(rootItem,names.at(i).toString(),i);
        tournaments.append(t);
    }


    beginInsertRows(QModelIndex(),root->childCount(),root->childCount() + tournaments.size()-1);
    root->addChildren(tournaments);
    endInsertRows();

    return true;
}

void TournamentsModel::onCurrentTournamentChanged(QModelIndex index)
{
    if(rootItemObject.childCount()>index.row()){
        const Tournament *t = rootItemObject.tournamentChild(index.row());
        if(t->childCount() > 0){//We only update the tournament from current tournament changed once to avoid spam, otherwise use some refresh button TODO: refresh button
            return;
        }

        qDebug() << "Getting tournament details for tournament " << t->name;

        c24Manager.getTournament(t->name);//Assumes we're already connected to the signal for receiving result
    }
}

void TournamentsModel::updateTournamentDetails(QJsonObject json)
{
}

