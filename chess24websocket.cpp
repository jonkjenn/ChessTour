#include "chess24websocket.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "chess24messages.h"
#include "chess24login.h"
#include "preparechess24ws.h"


Chess24Websocket::Chess24Websocket(QObject *parent, const QNetworkAccessManager &qnam, const Chess24Login &chess24Login, PrepareChess24WS &prepWS):
    QObject (parent),qnam(qnam), prepWS(prepWS), chess24Login(chess24Login)
{
    connect(&ws,&QWebSocket::connected,this,&Chess24Websocket::onConnected);
    connect(&ws,&QWebSocket::disconnected,this,&Chess24Websocket::onDisconnected);
    connect(&ws,&QWebSocket::sslErrors,this,&Chess24Websocket::sslErrors);
    connect(&ws,&QWebSocket::binaryMessageReceived,this,&Chess24Websocket::binaryMessageReceived);
    connect(&ws,static_cast<void(QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error),this,&Chess24Websocket::socketError);
    connect(&ws,&QWebSocket::textMessageReceived,this,&Chess24Websocket::onTextReceived);


    connect(&prepWS,&PrepareChess24WS::success,this,&Chess24Websocket::connectWS);
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

void Chess24Websocket::onLoggedInChanged()
{
    if(chess24Login.loggedIn()){
        userData = chess24Login.userData();
        prepWS.start();
    }
}

WSRequest* Chess24Websocket::sendMessage(QString msg, int messageId)
{
    QMap<int,WSRequest*>::iterator it = requests.insert(messageId,new WSRequest(messageId,this));

    qDebug() << "Sending message: " << msg;

    ws.sendTextMessage(msg);

    return it.value();
}

void Chess24Websocket::sendMessage(QString msg)
{
    ws.sendTextMessage(msg);
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

    case(Message::MessageType::connect):
    {

        int id = increaseAndGetMessageId();
        WSRequest *req = sendMessage(Chess24Messages::connect(userData,id),id);
        QObject::connect(req,&WSRequest::finished,[this](){
            emit connected();
        });
        return;
    }
    case(Message::MessageType::heartbeat):
    {
        qDebug() << "Sendt heartbeat";
        sendMessage(msg.data,messageId());//Heartbeat is same message in return
        return;
    }
    case(Message::MessageType::ack):
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
