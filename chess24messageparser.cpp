#include "chess24messageparser.h"
#include <QString>
#include <QtDebug>
#include "chess24messages.h"

using namespace Chess24Messages::Helpers;

Chess24MessageParser::Chess24MessageParser(QObject *parent):
    QObject(parent)
{

}

void Chess24MessageParser::parseMessage(QString msg)
{
    QString header = getHeader(msg);

    QStringList parts = header.split(":");

    bool ok;
    int msgType = parts[0].toInt(&ok);
    if(!ok){
        qDebug() << "Error in parsing websocket msg: ";
        return;
    }

    auto type = static_cast<Message::MessageType>(msgType);
    switch(type){
    case(Message::MessageType::disconnect):
        emit messageParsed(Message(type));
        break;
    case(Message::MessageType::connect):
    {
        emit messageParsed(Message(type));
        return;
    }
    case(Message::MessageType::heartbeat):
        emit messageParsed(Message(type,msg));
        //ws.sendTextMessage(msg);
        return;
    case(Message::MessageType::message):
        emit messageParsed(Message(type));
        return;
    case(Message::MessageType::json):
        emit messageParsed(Message(type));
        return;
    case(Message::MessageType::event):
        emit messageParsed(Message(type));
        return;
    case(Message::MessageType::ack):
    {
        //Id to the left of "+" and data to the right

        QString data = getData(msg);

        int ipluss = data.indexOf("+");
        if(ipluss<0){return;}

        QString before = data.mid(0,ipluss);

        bool ok = false;
        int id = before.toInt(&ok);//Get the msg id
        if(!ok){return;}


        QString json = data.mid(ipluss+1,data.size()-ipluss+1);

        emit messageParsed(Message(type,json,id));

        return;
    }
    case(Message::MessageType::error):
        emit messageParsed(Message(type));
        return;
    case(Message::MessageType::noop):
        emit messageParsed(Message(type));
        return;
    }

}
