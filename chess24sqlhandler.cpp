#include "chess24sqlhandler.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QJsonObject>
#include <QSettings>
#include <QDateTime>

#include "chess24manager.h"

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

Chess24SqlHandler::Chess24SqlHandler(QObject *parent, QSqlDatabase database, TournamentsSqlModel *tsm, Chess24Manager &c24Manager)
    :QObject(parent),database(database), tsm(tsm), c24Manager(c24Manager)
{

}

void Chess24SqlHandler::onCurrentTournamentChanged(int row)
{
    int pk = tsm->getPk(row);
    QSqlQuery checkSql(database);
    checkSql.prepare("SELECT LastUpdated FROM Tournament WHERE Id = ?");
    checkSql.addBindValue(pk);
    checkSql.exec();
    if(!checkSql.next()){return;}
    QDateTime lastUpdated = QDateTime::fromString(checkSql.value(0).toString());
    if(lastUpdated.isValid()){
        if(lastUpdated.daysTo(QDateTime::currentDateTimeUtc())<1){//Don't automatically update tournaments which already has been updated the last day, instead user has to click button. TODO: add button
            return;
        }
    }
    int column = tsm->fieldIndex("Name");
    QModelIndex index = tsm->index(row,column);
    QString name = tsm->data(index,Qt::DisplayRole).toString();
    c24Manager.getTournament(name);//Assumes we're already connected to the signal for receiving result*/
}

bool Chess24SqlHandler::addTournaments(QVariantList names)
{
    database.transaction();
    QSqlQuery query(database);
    query.prepare("INSERT OR IGNORE INTO Tournament (Name) VALUES (?)");
    query.addBindValue(names);
    if(!query.execBatch()){
        qDebug() << query.lastError();
        return false;
    }
    database.commit();
    QSqlQuery q(database);
    q.exec("UPDATE GlobalState SET TournamentUpdated = datetime()");
    qDebug() << q.lastError();

    QSettings settings;
    settings.setValue("database/tournamentUpdated",QDateTime::currentDateTimeUtc());
    tsm->forceRefresh();

    return true;
}

void Chess24SqlHandler::updatePlayers(const QJsonObject &players){
    database.transaction();
    QSqlQuery q("INSERT OR IGNORE INTO Players (FideId,Name) VALUES (?,?);",database);
    QVariantList fideIds;
    QVariantList names;
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
        fideIds.append(fideId);
        QString playerName = player["name"].toString();
        names.append(playerName);
    }
    q.addBindValue(fideIds);
    q.addBindValue(players);
    q.execBatch();
    database.commit();
}

void Chess24SqlHandler::updateTournamentDetails(QJsonObject json)
{
    QString name = json["id"].toString();
    QSqlQuery pkSql("SELECT Id FROM Tournament WHERE Name = ?",database);
    pkSql.addBindValue(name);
    if(!pkSql.next()){ qDebug() << "Could not retrieve primary key for tournament: " << name;}
    int tournamentPk = pkSql.value(0).toInt();
    int row = tsm->getRow(tournamentPk);
    bool updated = false;

    if(!json.keys().contains("players")){
        qDebug() << "Could not find players, wrong json format.";
        return;
    }

    QJsonObject players = json["players"].toObject();
    updatePlayers(players);

    if(json.keys().contains("status")){
        //setData(tIndex,json["status"].toString(),static_cast<int>(Tournament::TournamentRoles::StatusRole));
        tsm->setData(tsm->index(row,tsm->fieldIndex("Status")),json["status"].toString(),Qt::EditRole);
        updated = true;
    }

    if(json.keys().contains("eventType")){
        //setData(tIndex,json["eventType"].toString(),static_cast<int>(Tournament::TournamentRoles::EventTypeRole));
        tsm->setData(tsm->index(row,tsm->fieldIndex("EventType")),json["eventType"].toString(),Qt::EditRole);
        updated = true;
    }

    if(json.keys().contains("rounds")){
        QJsonObject rounds = json["rounds"].toObject();
        updated = updateRounds(rounds,tournamentPk);
    }

    if(updated){
        QSqlQuery lastSql(database);
        lastSql.prepare("UPDATE Tournament SET LastUpdated = ? WHERE Id = ?");
        lastSql.addBindValue(QDateTime::currentDateTimeUtc().toString());
        lastSql.addBindValue(tournamentPk);
        lastSql.exec();
    }
}

bool Chess24SqlHandler::updateRounds(QJsonObject rounds,int tournamentPk){

    bool updated = false;
    for(auto roundKey:rounds.keys()){
        QJsonObject round = rounds[roundKey].toObject();
        int number = roundKey.toInt();//TODO:error handling
        QSqlQuery roundPk(database);//Could also just insert since will override if exist
        roundPk.prepare("SELECT Id FROM Round WHERE TournamentId = ? AND Number = ?");
        roundPk.addBindValue(tournamentPk);
        roundPk.addBindValue(number);
        roundPk.exec();

        int pk = -1;
        if(!roundPk.next()){//Insert round
            roundPk.clear();
            QSqlQuery insertRound(database);
            insertRound.prepare("INSERT INTO Round (TournamentId,Number) VALUES (?,?)");
            insertRound.addBindValue(tournamentPk);
            insertRound.addBindValue(number);
            insertRound.exec();
            pk = insertRound.lastInsertId().toInt();
            updated = true;
        }else{
            pk = roundPk.value(0).toInt();
        }

        Q_ASSERT(pk >= 0);

        QJsonObject matches = round["matches"].toObject();
        updated = updated || updateMatches(matches,pk);
    }
    return updated;
}

bool Chess24SqlHandler::updateMatches(QJsonObject matches,int roundPk){
    bool updated = false;
        for(auto matchKey:matches.keys()){
            //TODO: not assume that each match only have one game

            //  			      				"matches": {
            //  .toObject()						   		"1": {
            //	.value(games).toObject()				"games": {
            //	.begin().value().toObject()					    "1": {
            QJsonObject game = matches[matchKey].toObject()
                    .value(MatchStrings::games).toObject()
                    .begin().value().toObject();

            int number = matchKey.toInt();
            QString fullGameRKey;
            int whiteFide = 0;
            int blackFide = 0;

            if(game.keys().contains(MatchStrings::fullGameRKey)){
                fullGameRKey = game[MatchStrings::fullGameRKey].toString();
                //setData(matchIndex,game[MatchStrings::fullGameRKey].toString(),static_cast<int>(Match::MatchRoles::FullGameRKeyRole));
            }

            if(game.keys().contains(MatchStrings::player)){
                bool ok = false;
                whiteFide = game.value(MatchStrings::player).toObject().value(MatchStrings::white).toString().toInt(&ok);
                if(!ok){//Could maybe assume is names then
                   continue;
                }
                blackFide = game.value(MatchStrings::player).toObject().value(MatchStrings::black).toString().toInt(&ok);
                //setData(matchIndex,white,static_cast<int>(Match::MatchRoles::WhiteFideRole));
                //setData(matchIndex,black,static_cast<int>(Match::MatchRoles::BlackFideRole));
            }else{
                continue;
            }

            QSqlQuery insertMatch(database);
            insertMatch.prepare("INSERT INTO Match (WhiteFide, BlackFide, Number, fullGameRKey, RoundId) VALUES(?,?,?,?,?)");
            insertMatch.addBindValue(whiteFide);
            insertMatch.addBindValue(blackFide);
            insertMatch.addBindValue(number);
            insertMatch.addBindValue(fullGameRKey);
            insertMatch.addBindValue(roundPk);
            insertMatch.exec();
            updated = true;
        }
}
