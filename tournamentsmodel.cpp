#include "tournamentsmodel.h"
#include <QObject>
#include <QtNetwork>
#include <iostream>

#include "tournament.h"
#include "tournamentsitem.h"
#include "chess24manager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDir>

namespace MatchStrings{//TODO: replace with string literal of some sort
    QString fullGameRKey = "fullGameRKey";
    QString player = "player";
    QString white = "white";
    QString black = "black";
    QString gameStatus = "gameStatus";
    QString status = "status";
    QString clock = "clock";
    QString now = "now";
    QString engine = "engine";
    QString score = "score";
    QString games = "games";
}


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

    if(!parent.isValid()){
        return createIndex(row,column,root);
    }else{
        TournamentsItem *t = static_cast<TournamentsItem*>(parent.internalPointer());
        switch(t->ItemType){
        case TournamentsItem::ItemType::Root://Parent is a tournament
            return createIndex()
                    break;
        case TournamentsItem::ItemType::Tournament://Parent is a round
            break;
        case TournamentsItem::ItemType::Round://Parent is a match
            break;
        case TournamentsItem::ItemType::Match://Should never happen
            break;

        }
    }

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
    TournamentsItem *t = static_cast<TournamentsItem*>(index.internalPointer())->child(index.row());
    if(t->setData(role,value)){
        emit dataChanged(index,index,QVector{role});
    }
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
        TournamentsItem *tm = static_cast<TournamentsItem *>(parent.internalPointer())->child(parent.row());
        if(tm->childCount()>0){
            return true;
        }
    }
    else{
        if(rootItem->childCount()>0){
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
         tm = static_cast<TournamentsItem *>(parent.internalPointer())->child(parent.row());
    }

    beginInsertRows(parent,tm->childCount(),tm->childCount());
    tm->addChild(child);
    endInsertRows();

    return true;
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

void TournamentsModel::onCurrentTournamentChanged(int row)
{
    if(row<0){return;}
    if(row>=rootItem->childCount()){return;}

    const Tournament *t = rootItemObject.tournamentChild(row);
    if(t->childCount() > 0){//We only update the tournament from current tournament changed once to avoid spam, otherwise use some refresh button TODO: refresh button
        return;
    }

    qDebug() << "Getting tournament details for tournament " << t->name;

    c24Manager.getTournament(t->name);//Assumes we're already connected to the signal for receiving result
}

void TournamentsModel::updateTournamentDetails(QJsonObject json)
{
    QString name = json["id"].toString();


    int pos = rootItemObject.position(name);
    if(pos<0){
        qDebug() << "Could not find child with name " + name + " in root";
        return;
    }
    QModelIndex tIndex = index(pos,0,QModelIndex());
    if(!tIndex.isValid()){
        qDebug() << "Invalid tIndex";
        return;
    }

    Tournament *t = rootItemObject.item(name);
    if(t == nullptr){
        qDebug() << "Found no tournament with this name";
        return;
    }

    if(!json.keys().contains("players")){
        qDebug() << "Could not find players, wrong json format.";
        return;
    }

    QJsonObject players = json["players"].toObject();
    for(auto p:players){
        if(!p.isObject()){
            qDebug() << "Could not parse players, wrong json format.";
            return;
        }//TODO: better error handling
        QJsonObject player = p.toObject();
        if(!(player.keys().contains("fideId") && player.keys().contains("name"))){
            qDebug() << "Could not parse players, wrong json format.";
            continue;
        }
        bool ok = false;
        int fideId = player["fideId"].toString().toInt(&ok);
        if(!ok){
            qDebug() << "Could not parse player fideId, wrong json format.";
            continue;
        }
        QString playerName = player["name"].toString();
        if(t->players.contains(fideId)){//If update previous record
                if(t->players[fideId]==playerName){continue;}//Skip if exact same exist
                t->players.insert(fideId,player["name"].toString());
                //TODO: Need to emit datachanged for all the places where the previous name was used if want dynamic update in GUI
        }else{
            t->players.insert(fideId,player["name"].toString());
            //TODO: also possibly datachanged
        }
    }

    if(json.keys().contains("status")){
        setData(tIndex,json["status"].toString(),static_cast<int>(Tournament::TournamentRoles::StatusRole));
    }

    if(json.keys().contains("eventType")){
        setData(tIndex,json["eventType"].toString(),static_cast<int>(Tournament::TournamentRoles::EventTypeRole));
    }

    if(!json.keys().contains("rounds")){
        qDebug() << "json contains no rounds";
        return;
    }

    QJsonObject rounds = json["rounds"].toObject();
    for(auto roundKey:rounds.keys()){
        int number = roundKey.toInt();//TODO:error handling
        int pos = t->position(number);

        //If tournament does not have this round try to add
        if(pos<0){
            Round *r = new Round(t,number);
            if(!addChild(tIndex,static_cast<TournamentsItem*>(r))){
                continue;
            }
        }
        pos = t->position(number);
        if(pos<0){continue;}//Add failed

        QModelIndex roundIndex = index(pos,0,tIndex);
        if(!roundIndex.isValid()){
            qDebug() << "Roundindex invalid";
            return;
        }
        TournamentsItem *roundItem = static_cast<TournamentsItem*>(roundIndex.internalPointer())->child(roundIndex.row());
        if(roundItem->tourType != TournamentsItem::ItemType::Round){
            qDebug() << "Incorrect itemtype";
        }
        Round *roundPointer = static_cast<Round*>(roundItem);
        QJsonObject round = rounds[roundKey].toObject();

        if(!round.keys().contains("matches")){continue;}

        QJsonObject matches = round["matches"].toObject();

        for(auto matchKey:matches.keys()){
            //TODO: not assume that each match only have one game

            //  			      				"matches": {
            //  .toObject()						   		"1": {
            //	.value(games).toObject()				"games": {
            //	.begin().value().toObject()					    "1": {
            QJsonObject game = matches[matchKey].toObject()
                    .value(MatchStrings::games).toObject()
                    .begin().value().toObject();

            int id = matchKey.toInt();
            int matchPos = roundPointer->position(id);
            if(matchPos <0){
                Match *matchPointer = new Match(roundItem,id);
                if(!addChild(roundIndex,static_cast<TournamentsItem*>(matchPointer))){
                    continue;
                }
            }
            matchPos = roundPointer->position(id);
            if(matchPos <0){continue;}

            QModelIndex matchIndex = index(matchPos,0,roundIndex);

            if(game.keys().contains(MatchStrings::fullGameRKey)){
                setData(matchIndex,game[MatchStrings::fullGameRKey].toString(),static_cast<int>(Match::MatchRoles::FullGameRKeyRole));
            }

            if(game.keys().contains(MatchStrings::player)){
                bool ok = false;
                int white = game.value(MatchStrings::player).toObject().value(MatchStrings::white).toString().toInt(&ok);
                if(!ok){//Could maybe assume is names then
                   continue;
                }
                int black = game.value(MatchStrings::player).toObject().value(MatchStrings::black).toString().toInt(&ok);
                setData(matchIndex,white,static_cast<int>(Match::MatchRoles::WhiteFideRole));
                setData(matchIndex,black,static_cast<int>(Match::MatchRoles::BlackFideRole));
            }
        }
    }


}

