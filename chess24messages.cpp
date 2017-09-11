#include "chess24messages.h"
#include <QVariantMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
namespace Chess24Messages{

namespace Helpers{
const QString header= "5:::";

QString mapToString(QVariantMap map){
    return QJsonDocument(QJsonObject::fromVariantMap(map)).toJson(QJsonDocument::JsonFormat::Compact);
}

QString idHeader(int messageId){
    return "5:" + QString::number(messageId) + "+::";
}

int findDataStart(QString msg)
{
    int i1 = msg.indexOf(":");
    if(i1<0){return -1;}
    int i2 = msg.indexOf(":",i1+1);
    if(i2<0){return -1;}
    int i3 = msg.indexOf(":",i2+1);
    if(i3<0){return -1;}
    return i3+1;
}

QString getData(QString msg){
    return msg.mid(findDataStart(msg));
}

QString getHeader(QString msg){
    return msg.mid(0,findDataStart(msg));
}

QString buildData(QString name, QVariantList args)
{
    QVariantMap map;
    map.insert("name",name);
    map.insert("args",args);
    return mapToString(map);
}

}

using namespace Helpers;

QString heartBeat(){
    return "2::";
}

QString connect(UserData data, int messageId)
{
    QVariantList args;
    args.append(data.authToken);
    QVariantMap args2;
    args2.insert("isGuest",!data.isRegistered);
    args2.insert("isPremium",data.isPremium);
    args2.insert("isAdmin",false);
    args2.insert("userName",data.name);
    args.append(args2);

    return idHeader(messageId) + buildData("connect",args);
}

QString getTouramentIds(int messageId){
    return idHeader(messageId) + buildData("getTournamentIds",{"web"});
}

QString getWebTournament(int messageId, QString name){
    return idHeader(messageId) + buildData("get",{model_webTournament + name});
}

QString getBroadcastChessGame(int messageId,QString name){
    return idHeader(messageId) + buildData("get",{model_broadcastChessGame + name});
}

QString subscribeWebTournament(QString name){
    return header + buildData("subscribe",{model_webTournament + name});
}

QString unSubscribeWebTournament(QString name){
    return header + buildData("unsubscribe",{model_webTournament + name});
}

QString subscribeBroadcastChessGame(QString name){
    return header + buildData("subscibe",{model_broadcastChessGame + name});
}

QVariantList tournamentNamesFromJSON(QString data)
{
    QJsonDocument doc(QJsonDocument::fromJson(data.toUtf8()));
    QJsonArray wrapper = doc.array();
    QJsonArray arr = wrapper[0].toArray();
    return arr.toVariantList();
}

QVariantMap listsFromTree(QVariantMap &result, const QVariantMap &valid, const QVariantMap &root){


}

/*QVariantMap transformWebTournament(QVariantMap &result, const QVariantMap &valid,const QVariantMap &root){
    for(auto key:root.keys()){
        if(key == "players"){
            result.insert("players",QVariantMap());
            listsFromTree(result.value("players"),valid.value(key),)
        }else if(key == "rounds"){

        }
    }
}*/

QVariantMap transformWebTournamentPlayers(const QVariantMap &root){
    QVariantMap result;
    QVariantList names;
    QVariantList fideId;
    for(auto key:root.keys()){
        QVariantMap player = root.value(key).toMap();
        names.append(player.value("name").toString());
        fideId.append(player.value("fideId").toString());
    }
    result.insert("Name",names);
    result.insert("FideId",fideId);
    return result;
}

QVariantMap transformWebTournamentMatches(const QVariantMap &root){
    QVariantMap result;
    QVariantList matchNumber;
    QVariantList gameNumber;
    QVariantList games;
    for(auto matchKey:root.keys()){
        QVariantMap g = root.value(matchKey).toMap();
        QVariantMap m = g.value("games").toMap();
        for(auto gameKey:m.keys()){
            matchNumber.append(matchKey);
            gameNumber.append(gameKey);

            QVariantMap inGame = m.value(gameKey).toMap();
            QVariantMap outGame;

            parseSubtree(outGame,validWebTournamentGame(),inGame);
            outGame.insert("Number",matchKey.toInt());
            outGame.insert("GameNumber",gameKey.toInt());
            games.append(outGame);
        }
    }
    result.insert("Number",matchNumber);
    result.insert("GameNumber",gameNumber);
    result.insert("games",games);
    return result;
}

QVariantMap transformWebTournamentRounds(const QVariantMap &root,std::optional<QVariantMap> gamesPerMatch){
    QVariantMap result;
    QVariantList numbers;
    QVariantMap rounds;
    for(auto key:root.keys()){
        QVariantMap inRound = root.value(key).toMap();
        QVariantMap outRound;

        insertFlat(outRound,inRound,"startDate","StartDate");
        if(gamesPerMatch){
            outRound.insert("GamesPerMatch",gamesPerMatch.value().value(key));
        }

       if(inRound.contains("matches")){
            QVariantMap matches = transformWebTournamentMatches(inRound.value("matches").toMap());
            outRound.insert("matchWrapper",matches);
        }

        numbers.append(key.toInt());
        outRound.insert("Number",key.toInt());
        rounds.insert(key,outRound);
    }
    result.insert("Number",numbers);
    result.insert("rounds",rounds);
    return result;
}

void insertFlat(QVariantMap &out, const QVariantMap &root,const QString &name, const QString &column){
    if(root.contains(name)){
        out.insert(column,root.value(name));
    }
}

void insertNest(QVariantMap &out, const QVariantMap &root,const QVector<QString> &name, const QString &column){

    QVariantMap temp = root;
    for(auto n:name){
        if(!temp.contains(n)){return;}
        QVariant t = temp.value(n);
        if(t.type() != QVariant::Type::Map){
            out.insert(column,temp.value(n));
        }else{
            temp = t.toMap();
        }
    }
}

QVariantMap transformWebTournament(const QVariantMap &root){
    QVariantMap result;
        if(root.keys().contains("players")){
            QVariantMap players = transformWebTournamentPlayers(root.value("players").toMap());
            result.insert("players",players);
        }
        if(root.keys().contains("rounds")){

            std::optional<QVariantMap> gamesPerMatch;

            if(root.keys().contains("gamesPerMatch")){
                 gamesPerMatch = root.value("gamesPerMatch").toMap();

                /*for(auto k:gamesPerMatch.keys()){
                    QVariantMap &round  = qvariant_cast<QVariantMap>(rounds[k]);
                    round.insert("GamesPerMatch",gamesPerMatch.value(k));
                }*/
            }

            QVariantMap rounds = transformWebTournamentRounds(root.value("rounds").toMap(),gamesPerMatch);


            result.insert("roundWrapper",rounds);
        }

        insertFlat(result,root,"currentRound","CurrentRound");
        insertNest(result,root,{"titles","en"},"Title");
        insertNest(result,root,{"descriptions","en"},"Description");
        insertFlat(result,root,"eventType","EventType");
        insertFlat(result,root,"status","Status");
        insertFlat(result,root,"id","Name");
        if(root.contains("currentGame")){
            insertFlat(result,root,"currentGame","CurrentGame");
        }


        return result;
}

void parseSubtree(QMap<QString,QVariant> &updates,const QVariantMap &valid,const QVariantMap &root){
    for(auto key:root.keys()){
        if(valid.contains(key)){
            if(valid.value(key).type() == QVariant::Type::Map){
                parseSubtree(updates,valid.value(key).toMap(),root.value(key).toMap());
            }else{
                //Use the corrected column name from valid map
                updates.insert(valid.value(key).toString(),root.value(key));
            }
        }
    }
}

QVariantMap validWebTournamentGame(){
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
    valid.insert("fullGameRKey","FullGameRKey");
    valid.insert("lastMoves","LastMoves");
    return valid;
}

}
