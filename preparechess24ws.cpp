#include "preparechess24ws.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

PrepareChess24WS::PrepareChess24WS(QObject *parent, QNetworkAccessManager &qnam):
    QObject(parent),qnam(qnam)
{
}

void PrepareChess24WS::start()
{
    //Get notification server
    QNetworkReply *reply = qnam.get(QNetworkRequest(QUrl("https://chess24.com/api/web/NodeServerAPI/getNotificationServer")));

    //Callback when the http request returns
    connect(reply,&QNetworkReply::finished,[reply,this]{
        this->notificationRespons(reply);
    });

    //Callback if the http request gets error
    connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
            [reply,this]{
        emit this->failure(reply->errorString());
    }
    );
}

void PrepareChess24WS::notificationRespons(QNetworkReply *reply)
{
    //Should get an error signal to handle the error, we just abort here.
    if(reply->error()>0){
        return;
    }

    //Read the response data from the server
    QByteArray data = reply->readAll();

    //Get the notification server from the data
    notificationServer = parseNotficationServer(data);

    //Abort and trigger failure if parsing fails
    if(notificationServer.isEmpty()){
        emit failure("Could not parse notification server.");
        return;
    }
    qDebug() << notificationServer;

    //Call the notificationServer we just found
    QNetworkReply *rep = qnam.get(QNetworkRequest(QUrl("https://" + notificationServer + "/socket.io/1/")));

    //Callback when request returns
    connect(rep,&QNetworkReply::finished,[rep,this](){this->wssidRespons(rep);});

    //Callback if error
    connect(rep, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
            [rep,this]{
        //Emit a failure signal
        emit this->failure(rep->errorString());
    }
    );
}

void PrepareChess24WS::wssidRespons(QNetworkReply *reply)
{
    //Should get an error signal to handle the error
    if(reply->error()>0){
        return;
    }

    QByteArray data = reply->readAll();
    qDebug() << data;
    wssId = parseWSSId(data);
    qDebug() << wssId;
    if(wssId.isEmpty()){
        emit failure("Could not parse wssId.");
    }else{
        emit success(notificationServer,wssId);
    }
}

QString PrepareChess24WS::parseNotficationServer(QString data)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data.toUtf8()));
    if(jdoc.isNull()){
        return QString();
    }
    QJsonObject o = jdoc.object();
    if(!o.keys().contains("data")){
        return QString();
    }
    QJsonObject odata = o["data"].toObject();
    if(!odata.keys().contains("server")){
        return QString();
    }

    QJsonObject oserver = odata["server"].toObject();
    if(!oserver.contains("host")){
        return QString();
    }
    return oserver["host"].toString();
}

QString PrepareChess24WS::parseWSSId(QString data)
{
    if(!data.contains(":")){
        return QString();
    }
    return data.split(":")[0];
}
