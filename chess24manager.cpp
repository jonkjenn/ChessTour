#include "chess24manager.h"
#include "chess24websocket.h"
#include "chess24messages.h"
#include "chess24sqlhandler.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <optional>

Chess24Manager::Chess24Manager(QObject *parent, Chess24Websocket &c24ws)
    :QObject(parent),
    c24ws(c24ws)
{
}

WSRequest *Chess24Manager::sendMessage(std::function<QString(int)> msgFunc) const {

    int id = c24ws.increaseAndGetMessageId();
    return c24ws.sendMessage(msgFunc(id),id);
}

WSRequest *Chess24Manager::sendMessage(std::function<QString(int,QString)> msgFunc,QString msgData) const{

    int id = c24ws.increaseAndGetMessageId();
    return c24ws.sendMessage(msgFunc(id,msgData),id);
}

void Chess24Manager::sendMessage(QString msg) const
{
    c24ws.sendMessage(msg);
}
