#include "chess24messages.h"
#include <QVariantMap>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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





}
