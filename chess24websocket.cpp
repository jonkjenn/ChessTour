#include "chess24websocket.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Chess24Websocket::Chess24Websocket(QObject *parent,QNetworkAccessManager &qnam):
    QObject (parent),qnam(qnam)
{
    QObject::connect(&ws,&QWebSocket::connected,this,&Chess24Websocket::onConnected);
    QObject::connect(&ws,&QWebSocket::disconnected,this,&Chess24Websocket::onDisconnected);
    QObject::connect(&ws,&QWebSocket::sslErrors,this,&Chess24Websocket::sslErrors);
    QObject::connect(&ws,&QWebSocket::binaryMessageReceived,this,&Chess24Websocket::binaryMessageReceived);
    QObject::connect(&ws,static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),this,&Chess24Websocket::socketError);
    QObject::connect(&ws,&QWebSocket::textMessageReceived,this,&Chess24Websocket::onTextReceived);
}

void Chess24Websocket::connectWS(UserData data,QString notificationServer,QString wssId){
    userData = data;
    ws.open(QUrl("wss://" + notificationServer + "/socket.io/1/websocket/" + wssId + "/"));
}

bool Chess24Websocket::isConnected()
{
    return m_isConnected;
}

void Chess24Websocket::setIsConnected(bool val)
{
    m_isConnected = val;
}

QString Chess24Websocket::buildEvent(QString message_id, QString name, QVariantMap args)
{

    /*
        QVariantMap map;
        map.insert("name","connect");
        QVariantMap args2;
        args2.insert("isGuest",!userData.isPremium);
        args2.insert("isPremium",userData.isPremium);
        args2.insert("isAdmin",false);
        args2.insert("userName",userData.name);
        QJsonArray args;
        args.append(userData.authToken);
        args.append(QJsonObject::fromVariantMap(args2));
        map.insert("args",args);
        QJsonDocument response = QJsonDocument(QJsonObject::fromVariantMap(map));
        //ws.sendTextMessage("5:1+::" + QString::fromUtf8(response.toJson()));
        //qDebug().noquote() << "5:1+::" + response.toJson(QJsonDocument::Compact);
        */
}

void Chess24Websocket::onConnected()
{
    qDebug() << "Connected";
    setIsConnected(true);
}

void Chess24Websocket::onDisconnected()
{
    qDebug() << "Websocket disconnected";
    setIsConnected(false);
}

void Chess24Websocket::onTextReceived(QString message)
{
    qDebug() << "Got message: " << message;

    QStringList parts = message.split(":");

    bool ok;
    uint messageType = parts[0].toUInt(&ok);
    if(!ok){
        qDebug() << "Error in parsing websocket message: " << message;
        return;
    }

    switch(messageType){
    case(MessageType::disconnect):
        break;
    case(MessageType::connect):
        break;
    case(MessageType::heartbeat):
        break;
    case(MessageType::message):
        break;
    case(MessageType::json):
        break;
    case(MessageType::event):
        break;
    case(MessageType::ack):
        break;
    case(MessageType::error):
        break;
    case(MessageType::noop):
        break;
    }


    /*
    if(message == "1::"){
        QVariantMap map;
        map.insert("name","connect");
        QVariantMap args2;
        args2.insert("isGuest",!userData.isPremium);
        args2.insert("isPremium",userData.isPremium);
        args2.insert("isAdmin",false);
        args2.insert("userName",userData.name);
        QJsonArray args;
        args.append(userData.authToken);
        args.append(QJsonObject::fromVariantMap(args2));
        map.insert("args",args);
        QJsonDocument response = QJsonDocument(QJsonObject::fromVariantMap(map));
        //ws.sendTextMessage("5:1+::" + QString::fromUtf8(response.toJson()));
        //qDebug().noquote() << "5:1+::" + response.toJson(QJsonDocument::Compact);
    }*/
}

void Chess24Websocket::sslErrors(const QList<QSslError> errors)
{
    for(auto e:errors) {
        qDebug() << e.errorString();
    }
}

void Chess24Websocket::binaryMessageReceived(const QByteArray data)
{
    qDebug() << data;
}

void Chess24Websocket::socketError(QAbstractSocket::SocketError error)
{

    qDebug() << error;
}
