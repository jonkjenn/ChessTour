#ifndef MESSAGE_H
#define MESSAGE_H
#include <QString>
#include <QJsonObject>

enum class MessageType{
    disconnect = 0,
    connect = 1,
    heartbeat = 2,
    message = 3,
    json = 4,
    event = 5,
    ack = 6,
    error = 7,
    noop = 8
};

struct Message
{
    Message(MessageType type):type(type){}
    int id=-1;
    MessageType type;
    QString data;

};

#endif // MESSAGE_H
