#include "chess24websocket.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

#include "chess24messages.h"
#include "chess24login.h"
#include "preparechess24ws.h"
#include "tokencontainer.h"

Chess24Websocket::Chess24Websocket(QObject *parent, QWebSocket &ws, PrepareChess24WS &prepWS, TokenContainer &token, QTimer &msgQTimer):
    QObject (parent),ws(ws), prepWS(prepWS), token(token), msgQTimer(msgQTimer)
{
    connect(&ws,&QWebSocket::connected,this,&Chess24Websocket::onConnected);
    connect(&ws,&QWebSocket::disconnected,this,&Chess24Websocket::onDisconnected);
    connect(&ws,&QWebSocket::sslErrors,this,&Chess24Websocket::sslErrors);
    connect(&ws,&QWebSocket::binaryMessageReceived,this,&Chess24Websocket::binaryMessageReceived);
    connect(&ws,static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),this,&Chess24Websocket::socketError);
    connect(&ws,&QWebSocket::textMessageReceived,this,&Chess24Websocket::onTextReceived);

    connect(&prepWS,&PrepareChess24WS::success,this,&Chess24Websocket::connectWS);

    token.setParent(this);
    msgQTimer.setParent(this);
    connect(&msgQTimer,&QTimer::timeout,this,&Chess24Websocket::send);
    ws.setParent(this);

    /*tokenTimer.setInterval(2000);
    connect(&tokenTimer,&QTimer::timeout,[this]{
        if(tokens<maxTokens){
            tokens += 1;
        }
    });
    tokenTimer.start();*/

    /*msgQTimer.setInterval(1000);
    msgQTimer.start();*/

}


void Chess24Websocket::connectWS(QString notificationServer,QString wssId){
    ws.open(QUrl("wss://" + notificationServer + "/socket.io/1/websocket/" + wssId + "/"));
}

bool Chess24Websocket::isConnected()
{
    return m_isConnected;
}

/*WSRequest *Chess24Websocket::getTournamentIds()
{
    return sendMessage(Chess24Messages::getTouramentIds(m_messageId));
}*/

void Chess24Websocket::onUserdataChanged(UserData data)
{
    if(data.result){
        userData = data;
        prepWS.start();
        loggedIn = true;
    }else{
        loggedIn = false;
    }
}

void Chess24Websocket::send(){
    if(token.getToken()){
        if(msgQ.size()>0){
            qDebug() << "Sending message: " << msgQ.at(0);
            ws.sendTextMessage(msgQ.dequeue());
        }
    }
}

WSRequest* Chess24Websocket::sendMessage(QString msg, int messageId)
{
    WSRequest* req = requests.insert(messageId,new WSRequest(messageId,this)).value();

    if(loggedIn){
        msgQ.enqueue(msg);
    }else{
        //Return empty data in 1 second
        QTimer *t = new QTimer(this);
        t->setInterval(1000);
        t->setSingleShot(true);
        connect(t,&QTimer::timeout,[t,req](){
            req->data("");
            t->deleteLater();
        });
        t->start();
    }

    return req;
}

void Chess24Websocket::sendMessage(QString msg)
{
    if(loggedIn){
        msgQ.enqueue(msg);
    }
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
    emit messageReceived(message);
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

//Messages from messageparser
void Chess24Websocket::handleMessage(Message msg){
    switch(msg.type){

    case(MessageType::connect):
    {
        int id = increaseAndGetMessageId();
        WSRequest *req = sendMessage(Chess24Messages::connect(userData,id),id);
        QObject::connect(req,&WSRequest::finished,[this](){
            emit connected();
        });
        return;
    }
    case(MessageType::heartbeat):
    {
        qDebug() << "Sendt heartbeat";
        sendMessage(Chess24Messages::heartBeat());
        return;
    }
    case(MessageType::ack):
    {
        requests[msg.id]->data(msg.data);
        requests.remove(msg.id);
        return;
    }
    default:
        return;
    }
}

int Chess24Websocket::increaseAndGetMessageId(){
    m_messageId +=1;
    return m_messageId;
}

int Chess24Websocket::messageId()
{
    return m_messageId;
}
