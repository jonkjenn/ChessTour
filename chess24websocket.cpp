#include "chess24websocket.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "chess24messages.h"


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

WSRequest *Chess24Websocket::getTournamentIds()
{
   return createRequest(Chess24Messages::getTouramentIds(message_id));
}

WSRequest* Chess24Websocket::createRequest(QString msg)
{
    QMap<int,WSRequest*>::iterator it = requests.insert(message_id,new WSRequest(message_id,this));

    qDebug() << "Sending message: " << msg;

    ws.sendTextMessage(msg);

    message_id+= 1;

    return it.value();
}

void Chess24Websocket::setIsConnected(bool val)
{
    m_isConnected = val;
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
    QStringList parts = message.split(":");

    bool ok;
    int messageType = parts[0].toInt(&ok);
    if(!ok){
        qDebug() << "Error in parsing websocket message: ";
        return;
    }

    switch(messageType){
    case(MessageType::disconnect):
        break;
    case(MessageType::connect):
    {
        WSRequest *req = createRequest(Chess24Messages::connect(userData,message_id));
        QObject::connect(req,&WSRequest::finished,[this](){
            emit connected();
        });
        return;
    }
    case(MessageType::heartbeat):
        ws.sendTextMessage(message);
        return;
    case(MessageType::message):
        break;
    case(MessageType::json):
        break;
    case(MessageType::event):
        break;
    case(MessageType::ack):
    {
        //Id to the left of "+" and data to the right
        QStringList ackParts = parts[3].split("+");
        if(ackParts.length() != 2){return;}
        bool ok = false;
        int id = ackParts[0].toInt(&ok);//Get the message id
        if(!ok){return;}
        requests[id]->data(ackParts[1]);
        requests.remove(id);
        return;
    }
    case(MessageType::error):
        break;
    case(MessageType::noop):
        break;
    }


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
