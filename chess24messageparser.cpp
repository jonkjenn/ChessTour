#include "chess24messageparser.h"
#include <QString>
#include <QtDebug>

Chess24MessageParser::Chess24MessageParser(QObject *parent):
    QObject(parent)
{

}

void Chess24MessageParser::parseMessage(QString msg)
{
    QStringList parts = msg.split(":");

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
        QStringList ackParts = parts[3].split("+");
        if(ackParts.length() != 2){return;}
        bool ok = false;
        int id = ackParts[0].toInt(&ok);//Get the msg id
        if(!ok){return;}

        emit messageParsed(Message(type,ackParts[1],id));

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
