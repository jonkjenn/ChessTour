#include "chess24sqlhandler.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QJsonObject>
#include <QSettings>
#include <QDateTime>
#include <optional>
#include <QMap>

#include "chess24manager.h"

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

Chess24SqlHandler::Chess24SqlHandler(QObject *parent, QSqlDatabase database, TournamentsSqlModel &tsm, RoundsSqlModel &rsm, LiveMatchSqlModel &lsm)
    :QObject(parent),database(database), tsm(tsm),rsm(rsm),lsm(lsm)
{

}

void Chess24SqlHandler::onWebTournamentRedisAR(WebTournamentRedisAR msg){
    int tourRow = tsm.getRow(rsm.currentPK());
    QModelIndex nameIndex = tsm.index(tourRow,tsm.fieldIndex("Name"));
    QString currentTourName = tsm.data(nameIndex,Qt::DisplayRole).toString();
    if(msg.tournament != currentTourName){
        qDebug() << "Got webtourredis for tournament we are not watching, currently watching: " << currentTourName << " in msg: " << msg.tournament;
        return;
    }

    if(!msg.args.keys().contains("action")){
        qDebug() << "No action in msg";
        return;
    }

    if(msg.args.value("action").toString() != "set"){
        qDebug() << "Can only handle set action";
        return;
    }

    if(msg.args.keys().contains("diffs")){
        QJsonObject diffs = msg.args.value("diffs").toObject();
        if(diffs.keys().contains("rounds")){
            QJsonObject rounds = diffs.value("rounds").toObject();
            updateLiveRounds(rounds);
        }
    }
}

void Chess24SqlHandler::updateLiveRounds(QJsonObject rounds){
    for(auto roundKey:rounds.keys()){
        QJsonObject round = rounds.value(roundKey).toObject();

        //Get the Pk for this round from the database
        QSqlQuery q(database);
        q.prepare("SELECT Id FROM Round WHERE Number = :number AND TournamentId = :tournamentId");
        q.bindValue(":number",roundKey.toInt());
        q.bindValue(":tournamentId",rsm.currentPK());
        q.exec();
        if(!q.next()){qDebug() << "Could not find pk"; continue;}

        int roundPk = q.value(0).toInt();

        if(round.keys().contains("matches")){
            QJsonObject matches = round.value("matches").toObject();
            updateLiveMatches(matches,roundPk);
        }
    }
}

void Chess24SqlHandler::updateLiveMatches(QJsonObject matches,int roundPk){

    for(auto matchKey:matches.keys()){
        QJsonObject match = matches.value(matchKey).toObject();
        if(match.keys().contains("games")){
            QJsonObject games = match.value("games").toObject();
            for(auto gameKey:games.keys()){
                QJsonObject game = games.value(gameKey).toObject();
                int matchNumber = matchKey.toInt();
                int gameNumber = gameKey.toInt();

                updateLiveGame(matchNumber,gameNumber,game,roundPk);
            }
        }
    }
}

//Set data in MatchModel only
void Chess24SqlHandler::setData(QMap<QString,QVariant> &updates, int rowId,QString column,QVariant value){
    updates.insert(column,value);
    lsm.setData(rowId,column,value);
}

void Chess24SqlHandler::updateLiveGame(int matchNumber, int gameNumber, QJsonObject game, int roundPk){
    //If this round is the current shown round,
    //update model if true, update database if false.
    bool live = roundPk == lsm.currentPk();
    //Get Pk for match/game combo
    QSqlQuery q(database);
    q.prepare("SELECT Id FROM Match WHERE Number = :matchNumber AND GameNumber = :gameNumber AND RoundId = :roundId");
    q.bindValue(":matchNumber",matchNumber);
    q.bindValue(":gameNumber",gameNumber);
    q.bindValue(":roundId",roundPk);
    q.exec();

    if(!q.next()){ qDebug() << "Could not find PK for match/game"; return;}
    int rowId = q.value(0).toInt();

    //Key is column, Value is value, each element gets added to a SQL call
    QMap<QString,QVariant> u;

    QJsonObject root = game;
    QVariantMap valid;
    QVariantMap engine;
    engine.insert("score","EngineScore");
    engine.insert("mate","EngineMate");
    valid.insert("engine",engine);

    QVariantMap player;
    player.insert("black","BlackFide");
    player.insert("white","WhiteFide");
    valid.insert("player",player);

    QVariantMap gameStatus;
    QVariantMap result;
    result.insert("white", "ResultWhite");
    result.insert("black", "ResultBlack");
    gameStatus.insert("result",result);
    gameStatus.insert("status","Status");

    valid.insert("gameStatus",gameStatus);

    valid.insert("currentFen","CurrentFEN");
    valid.insert("earlierFen","EarlierFEN");

    parseSubtree(u,valid,root);

    if(live){
        updateLiveGameFromUpdates(u,rowId);
    }else{
        updateDbGameFromUpdates(u,matchNumber,gameNumber,roundPk);
    }
}

void Chess24SqlHandler::updateDbGameFromUpdates(const QMap<QString,QVariant> &updates, int matchNumber, int gameNumber, int roundPk){
    QSqlQuery q(database);
    QString sql("UPDATE Match SET ");
    for(auto key:updates.keys()){
        sql.append(" " + key + " = :" + key + ",");
    }
    sql.remove(sql.length()-1,1);//Remove last ,
    sql.append(" WHERE Number = :number AND GameNumber = :gameNumber AND RoundId = :roundId");
    q.prepare(sql);
    for(auto key:updates.keys()){
        q.bindValue(":"+key,updates.value(key));
    }
    q.bindValue(":number",matchNumber);
    q.bindValue(":gameNumber",gameNumber);
    q.bindValue(":roundId",roundPk);
    if(!q.exec()){
        qDebug() << q.lastError();
        qDebug() << q.lastQuery();
    }
}

void Chess24SqlHandler::updateLiveGameFromUpdates(const QMap<QString,QVariant> &updates,int rowId){
    for(auto key:updates.keys()){
        lsm.setData(rowId,key,updates.value(key));
        //msm.setDataNoDataChanged(rowId,key,updates.value(key));
    }
}

//Recursively add to list of updates based on the valid attributes in valid map
void Chess24SqlHandler::parseSubtree(QMap<QString,QVariant> &updates,const QVariantMap &valid,const QJsonObject &root){
    for(auto key:root.keys()){
        if(valid.contains(key)){
            if(valid.value(key).type() == QVariant::Type::Map){
                parseSubtree(updates,valid.value(key).toMap(),root.value(key).toObject());
            }else{
                //Use the corrected column name from valid map
                updates.insert(valid.value(key).toString(),root.value(key).toVariant());
            }
        }
    }
}

QDateTime Chess24SqlHandler::lastUpdated(int row){
    int pk = tsm.getPk(row);
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
    //query.addBindValue(order);

        QSqlQuery query(database);
        query.prepare("INSERT OR IGNORE INTO Tournament (Name) VALUES (?)");
        query.addBindValue(names);

        if(!query.execBatch()){
            qDebug() << query.lastError();
            database.rollback();
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
    tsm.forceRefresh();

    return true;
}

void Chess24SqlHandler::updatePlayers( QJsonObject &players) {
    database.transaction();
    QSqlQuery q(database);
    q.prepare("INSERT OR IGNORE INTO Players (FideId,Name) VALUES (?,?);");
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
        QString fideId = player["fideId"].toString();
        fideIds.append(fideId);
        QString playerName = player["name"].toString();
        names.append(playerName);
    }
    if(fideIds.size() != names.size()){
        database.rollback();
        return;
    }
    q.addBindValue(fideIds);
    q.addBindValue(names);
    q.execBatch();
    database.commit();
}

void Chess24SqlHandler::updateTournamentDetails(QJsonObject json)
{
    if(!json.contains("id")){
        return;
    }
    QString name = json["id"].toString();
    QSqlQuery pkSql(database);
    pkSql.prepare("SELECT Id FROM Tournament WHERE Name = :name");
    pkSql.bindValue(":name",name);
    pkSql.exec();
    if(!pkSql.next()){ qDebug() << "Could not retrieve primary key for tournament: " << name; qDebug() << pkSql.lastError();}
    int tournamentPk = pkSql.value(0).toInt();
    int row = tsm.getRow(tournamentPk);
    bool updated = false;

    if(json.keys().contains("titles")){
        QJsonObject titles = json.value("titles").toObject();
        if(titles.keys().contains("en")){
            tsm.setData(row,"Title",titles.value("en").toString());
        }
    }

    if(json.keys().contains("currentRound")){
        tsm.setData(row,"CurrentRound",json.value("currentRound").toInt());
    }

    if(!json.keys().contains("players")){
        qDebug() << "Could not find players, wrong json format.";
        return;
    }

    QJsonObject players = json["players"].toObject();
    updatePlayers(players);

    if(json.keys().contains("status")){
        tsm.setData(tsm.index(row,tsm.fieldIndex("Status")),json["status"].toString(),Qt::EditRole);
        updated = true;
    }

    if(json.keys().contains("eventType")){
        tsm.setData(tsm.index(row,tsm.fieldIndex("EventType")),json["eventType"].toString(),Qt::EditRole);
        updated = true;
    }

    if(json.keys().contains("rounds")){
        QJsonObject rounds = json["rounds"].toObject();
        updated = updateRounds(rounds,tournamentPk);
    }

    if(updated){
        QSqlQuery lastSql(database);
        lastSql.prepare("UPDATE Tournament SET LastUpdated = :lastUpdated WHERE Id = :id");
        lastSql.bindValue(":lastUpdated",QDateTime::currentDateTimeUtc().toString());
        lastSql.bindValue(":id",tournamentPk);
        lastSql.exec();

        tsm.forceRefresh();
        rsm.forceRefresh();
        int roundPk = rsm.getPk(rsm.currentShownRound());
        qDebug() << "roundPk" << roundPk;
        lsm.setRound(roundPk);
    }
}

bool Chess24SqlHandler::updateRounds(QJsonObject rounds,int tournamentPk){

    bool updated = false;
    for(auto roundKey:rounds.keys()){
        QJsonObject round = rounds[roundKey].toObject();
        int number = roundKey.toInt();//TODO:error handling
        QSqlQuery roundPk(database);//Could also just insert since will override if exist
        roundPk.prepare("SELECT Id FROM Round WHERE TournamentId = :tournamentId AND Number = :number");
        roundPk.bindValue(":tournamentId",tournamentPk);
        roundPk.bindValue(":number",number);
        roundPk.exec();

        int pk = -1;
        if(!roundPk.next()){//Insert round
            roundPk.clear();
            QSqlQuery insertRound(database);
            insertRound.prepare("INSERT INTO Round (TournamentId,Number) VALUES (:tournamentId,:number)");
            insertRound.bindValue(":tournamentId",tournamentPk);
            insertRound.bindValue(":number",number);
            insertRound.exec();
            pk = insertRound.lastInsertId().toInt();
            updated = true;
        }else{
            pk = roundPk.value(0).toInt();
        }
        if(round.keys().contains("startDate")){
            QSqlQuery q(database);
            q.prepare("UPDATE Round SET StartDate = :startDate WHERE TournamentId = :tournamentId and Id = :id");
            q.bindValue(":startDate",round.value("startDate").toString());
            q.bindValue(":tournamentId",tournamentPk);
            q.bindValue(":id",pk);
            q.exec();
            if(!q.exec()){
                qDebug() << "Could not update startdate";
            }
        }

        Q_ASSERT(pk >= 0);

        QJsonObject matches = round["matches"].toObject();
        //.value("1").toObject()
        //.value("games").toObject();

        updateMatches(matches,pk);
        updated = updated || updateMatches(matches,pk);
    }
    return updated;
}

std::optional<int> Chess24SqlHandler::getInt(QJsonObject source, QString key)
{
    if(source.keys().contains(key)){
        return source[key].toInt();
    }
    return {};
}

std::optional<QVariant> Chess24SqlHandler::getString(QJsonObject source, QString key)
{
    if(source.keys().contains(key)){
        return source[key].toVariant();
    }
    return {};
}

bool Chess24SqlHandler::updateMatches(QJsonObject matches,int roundPk){
    bool updated = false;
    QSqlQuery deleteMatches(database);
    deleteMatches.prepare("DELETE FROM Match WHERE RoundId = :roundId");
    deleteMatches.bindValue(":roundId",roundPk);
    deleteMatches.exec();

    database.transaction();
    for(auto matchKey:matches.keys()){
        int matchNumber = matchKey.toInt();

        //  			      				"matches": {
        //  .toObject()						   		"1": {
        //	.value(games).toObject()				"games": {
        QJsonObject games = matches[matchKey].toObject()
                .value(MStr::games).toObject();


        for(auto gameKey:games.keys()){

            auto game = games[gameKey].toObject();

            int gameNumber = gameKey.toInt();
            QVariant fullGameRKey = getString(game,MStr::fullGameRKey).value_or("");
            QVariant status;
            QVariant whiteFide = 0;
            QVariant blackFide = 0;
            QVariant resultWhite = QVariant(QVariant::String);
            QVariant resultBlack = QVariant(QVariant::String);
            QVariant currentFen = getString(game,MStr::currentFen).value_or("");
            QVariant earlierFen = getString(game,MStr::earlierFen).value_or("");
            QVariant lastMoves = QVariant(QVariant::String);

            if(game.keys().contains(MStr::player)){
                whiteFide = game.value(MStr::player).toObject().value(MStr::white).toString();
                blackFide = game.value(MStr::player).toObject().value(MStr::black).toString();
            }else{
                //Need to have players in a game
                continue;
            }

            /*"gameStatus": {
              "result": {
                "white": 1,
                "black": 0
              },
              "status": "finished"
            }*/
            if(game.keys().contains(MStr::gameStatus)){
                auto gameStatusObj = game[MStr::gameStatus].toObject();
                if(gameStatusObj.keys().contains(MStr::result)){
                    auto resultObj = gameStatusObj[MStr::result].toObject();
                    resultWhite = getString(resultObj,MStr::white).value_or(QVariant(QVariant::String));
                    resultBlack = getString(resultObj,MStr::black).value_or(QVariant(QVariant::String));
                }
                status = getString(gameStatusObj,MStr::status).value_or("");
            }

            QSqlQuery insertMatch(database);
            insertMatch.prepare("INSERT INTO Match "
                                "(WhiteFide, BlackFide, Number, GameNumber, FullGameRKey, RoundId,  ResultWhite,  ResultBlack,  CurrentFEN, EarlierFEN, Status) "
                                "VALUES (:whiteFide, :blackFide,:number,:gameNumber,:fullGameRKey,:roundId, :resultWhite, :resultBlack, :currentFEN, :earlierFEN,:status)");
            insertMatch.bindValue(":whiteFide",whiteFide);
            insertMatch.bindValue(":blackFide",blackFide);
            insertMatch.bindValue(":number",matchNumber);
            insertMatch.bindValue(":gameNumber",gameNumber);
            insertMatch.bindValue(":fullGameRKey",fullGameRKey);
            insertMatch.bindValue(":roundId",roundPk);
            insertMatch.bindValue(":resultWhite",resultWhite);
            insertMatch.bindValue(":resultBlack",resultBlack);
            insertMatch.bindValue(":currentFEN",currentFen);
            insertMatch.bindValue(":earlierFEN",earlierFen);
            insertMatch.bindValue(":status",status);
            insertMatch.exec();
            if(insertMatch.lastError().isValid()){
                qDebug() << insertMatch.lastError();
                qDebug() << insertMatch.lastQuery();
            }else{
                updated = true;
            }
        }

    }
    database.commit();
    return updated;
}
