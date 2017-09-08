#include "chess24sqlhandler.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QJsonObject>
#include <QSettings>
#include <QDateTime>
#include <optional>
#include <QMap>

#include "sqlhelper.h"

namespace MStr{//TODO: replace with string literal of some sort
static QString fullGameRKey = "fullGameRKey";
static QString player = "player";
static QString white = "white";
static QString black = "black";
static QString gameStatus = "gameStatus";
static QString status = "status";
static QString clock = "clock";
static QString now = "now";
static QString engine = "engine";
static QString score = "score";
static QString games = "games";
static QString result = "result";
static QString currentFen = "currentFen";
static QString earlierFen = "earlierFen";
static QString lastMoves = "lastMoves";
}
using namespace std;

Chess24SqlHandler::Chess24SqlHandler(QObject *parent)
    :QObject(parent)
{
}

QVariantList Chess24SqlHandler::getMatchPks(QVariantList Number, QVariantList GameNumber, int roundPk) {
    QVariantMap whereList;
    whereList.insert("Number",Number);
    whereList.insert("GameNumber",GameNumber);
    QVariantMap values;
    values.insert("RoundId",roundPk);
    return SqlHelper::getColumnList(database,"Match","Id",Number.size(),whereList,values);
}

QVariantList Chess24SqlHandler::insertUpdateMatches(const QVariantList &Number,const QVariantList &GameNumber, const QVariantList &games, const int roundPk,bool returnChanges){
    QVariantList changes;

    QVector<QVariant> pkVector = QVector<QVariant>(Number.size(),QVariant::fromValue(roundPk));
    QVariantList pkList = QList<QVariant>::fromVector(pkVector);

    SqlHelper::insertLists(database,
                           "Match",
                            {Number,GameNumber,pkList},
                            {"Number", "GameNumber", "RoundId"}
                           );

    //QVariantList pks = getMatchPks(Number,GameNumber,roundPk);

    for(int i=0;i<games.size();++i){
        QVariantMap game = games.at(i).toMap();
        QVariantMap where;
        //where.insert("Id",pks.at(i));
        where.insert("RoundId",roundPk);
        where.insert("Number",game.value("Number").toInt());
        where.insert("GameNumber",game.value("GameNumber").toInt());
        SqlHelper::updateTable(database,"Match",game,where);

        if(returnChanges){
            QVariantList c;
            c.append(roundPk);
            c.append(game.value("Number"));
            c.append(game.value("GameNumber"));
            for(auto k:game.keys()){
                c.append(k);
            }
            changes.push_back(c);
        }
    }

    return changes;
}


//TODO:: This function does too much
QVariantMap Chess24SqlHandler::insertUpdateRounds(const QVariantList &Number, const QVariantMap &rounds, const int tournamentPk, bool returnChanges){

    QVariantMap changes;
    QVariantList roundChanges;
    QVariantList matchChanges;

    QVector<QVariant> pkVector = QVector<QVariant>(Number.size(),QVariant::fromValue(tournamentPk));
    QVariantList pkList = QList<QVariant>::fromVector(pkVector);

    SqlHelper::insertLists(database,
                           "Round",
                           {Number,pkList },
                            {"Number","TournamentId"}
                );

    //QVariantList pks = getRoundPk(Number,tournamentPk);
    QVariantMap lists;
    lists.insert("Number", Number);
    QVariantMap vals;
    vals.insert("TournamentId",tournamentPk);
    QVariantList pks = SqlHelper::getColumnList(database,"Round","Id",rounds.size(),lists,vals);

    database.transaction();

    for(size_t i=0;i<rounds.keys().size();++i){
        QVariantMap round = rounds.value(rounds.keys().at(i)).toMap();
        QVariantMap where;
        where.insert("Id",pks.at(i));
        SqlHelper::updateTable(database,"Round",round,where);

        if(returnChanges){
            QVariantList c;
            c.append(pks.at(i));
            for(auto k:round.keys()){
                c.append(k);
            }
        }
        QVariantMap matchWrapper = round.value("matchWrapper").toMap();

        QVariantList mc = insertUpdateMatches(
                    matchWrapper.value("Number").toList(),
                    matchWrapper.value("GameNumber").toList(),
                    matchWrapper.value("games").toList(),
                    pks.at(i).toInt(),
                    returnChanges
                    );
        if(returnChanges){
            matchChanges.push_back(mc);
        }
    }

    database.commit();

    if(returnChanges){
        changes.insert("round",roundChanges);
        changes.insert("match",matchChanges);
        return changes;
    }
    return QVariantMap();
}

void Chess24SqlHandler::insertUpdatePlayers(QVariantList Name, QVariantList FideId){
    QSqlQuery q(database);
    q.prepare("insert or ignore into Players (Name, FideId) values (?,?)");
    q.addBindValue(Name);
    q.addBindValue(FideId);
    if(!q.execBatch()){qDebug() << q.lastError();}
}

optional<int> Chess24SqlHandler::getGamePk(QString number, QString gameNumber, int roundId){
    QSqlQuery q(database);
     q.prepare("select Id from Match where Number = ? and GameNumber = ? and RoundId = ?");
     q.addBindValue(number);
     q.addBindValue(gameNumber);
     q.addBindValue(roundId);
     if(!q.exec()){qDebug() << q.lastError();}
     if(q.next()){
         return q.value(0).toInt();
     }
     return {};
}


QVariantList Chess24SqlHandler::getRoundPk(QVariantList number, int tournamentId) {
    QVariantList pks;
    for(auto n:number){
        QSqlQuery q(database);
        q.prepare("select Id from Round where Number = ? and TournamentId = ?");
        q.addBindValue(n);
        q.addBindValue(tournamentId);
        if(!q.exec()){qDebug() << q.lastError();}
        if(q.next()){
            pks.append(q.value(0));
        }
    }
    return pks;
}

optional<int> Chess24SqlHandler::getRoundPk(QString number, int tournamentId){
    QSqlQuery q(database);
     q.prepare("select Id from Round where Number = ? and TournamentId = ?");
     q.addBindValue(number);
     q.addBindValue(tournamentId);
     if(!q.exec()){qDebug() << q.lastError();}
     if(q.next()){
         return q.value(0).toInt();
     }
     return {};
}

optional<int> Chess24SqlHandler::getTournamentPk(QString name) {
    QSqlQuery q(database);
     q.prepare("select Id from Tournament where Name = ?");
     q.addBindValue(name);
     if(!q.exec()){qDebug() << q.lastError();}
     if(q.next()){
         return q.value(0).toInt();
     }
     return {};
}

QVariantMap Chess24SqlHandler::updateTournament(QString name, QVariantMap map, bool returnChanges) {
    QVariantMap changes;

    database.transaction();

    optional<int> tournamentPk = getTournamentPk(name);
    if(!tournamentPk){return changes;}

    if(map.keys().contains("players")){
        QVariantMap playerWrapper = map.value("players").toMap();
        insertUpdatePlayers(playerWrapper.value("Name").toList(),playerWrapper.value("FideId").toList());
    }
    if(map.keys().contains("roundWrapper")){
        QVariantMap roundWrapper = map.value("roundWrapper").toMap();
        changes = insertUpdateRounds(roundWrapper.value("Number").toList(),roundWrapper.value("rounds").toMap(),tournamentPk.value(),returnChanges);
    }

    QVariantMap where;
    where.insert("Id",tournamentPk.value());
    SqlHelper::updateTable(database,"Tournament",map,where);

    if(returnChanges){
        changes.insert("TournamentPk",tournamentPk.value());
        QVariantList c;
        for(auto k:map.keys()){
            c.append(k);
        }
        changes.insert("tournament",c);
    }

    QSqlQuery lastSql(database);
    lastSql.prepare("UPDATE Tournament SET LastUpdated = :lastUpdated WHERE Id = :id");
    lastSql.bindValue(":lastUpdated",QDateTime::currentDateTimeUtc().toString());
    lastSql.bindValue(":id",tournamentPk.value());
    lastSql.exec();

    database.commit();

    return changes;
}

QDateTime Chess24SqlHandler::lastUpdated(int pk){
    QSqlQuery checkSql(database);
    checkSql.prepare("SELECT LastUpdated FROM Tournament WHERE Id = :id");
    checkSql.bindValue(":id",pk);
    checkSql.exec();
    if(!checkSql.next()){qDebug() <<checkSql.lastError();return QDateTime();}
    return QDateTime::fromString(checkSql.value(0).toString());
}

bool Chess24SqlHandler::addTournaments(QVariantList names)
{

    database.transaction();
    QSqlQuery query(database);
    query.prepare("INSERT OR IGNORE INTO Tournament (Name) VALUES (:names)");
    query.bindValue(":names",names);

    if(!query.execBatch()){
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
        return false;
    }
    database.commit();

    database.transaction();
    QVariantList order;
    for(int i=0;i<names.size();++i){
        order.append(i);
    }

    QSqlQuery q(database);
    q.prepare("UPDATE Tournament SET OriginalOrder = ? WHERE Name = ?");
    q.addBindValue(order);
    q.addBindValue(names);
    if(!q.execBatch()){
        qDebug() << q.lastError();
        database.rollback();
        return true;
    }
    database.commit();

    QSettings settings;
    settings.setValue("database/tournamentUpdated",QDateTime::currentDateTimeUtc());

    return true;
}
