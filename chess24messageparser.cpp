#include "chess24messageparser.h"
#include <QString>
#include <QtDebug>
#include "chess24messages.h"
#include <QJsonDocument>
#include <QJsonArray>

using namespace Chess24Messages::Helpers;

Chess24MessageParser::Chess24MessageParser(QObject *parent):
    QObject(parent)
{

}

void Chess24MessageParser::parseEvent(const QString &model,const QJsonObject &root){
        if(model.startsWith("webTournamentRedisAR:")){
            QString tournament = model.mid(model.indexOf(":")+1);

            if(!root.keys().contains("args")){
                qDebug() << "Event does not contain args";
                return;
            }

            QJsonArray argsWrapper = root.value("args").toArray();
            WebTournamentRedisAR tourRedis;
            tourRedis.tournament = tournament;
            tourRedis.args = argsWrapper.at(0).toObject();

            emit webTournamentRedisAR(tourRedis);
        }
        else{
            qDebug() << "Unknown model name";
        }
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

    auto type = static_cast<MessageType>(msgType);
    switch(type){
    case(MessageType::disconnect):
        emit messageParsed(Message(type));
        break;
    case(MessageType::connect):
    {
        emit messageParsed(Message(type));
        return;
    }
    case(MessageType::heartbeat):
    {
        Message m(type);
        m.data = msg;
        emit messageParsed(m);
    }
        return;
    case(MessageType::message):
        emit messageParsed(Message(type));
        return;
    case(MessageType::json):
        emit messageParsed(Message(type));
        return;
    case(MessageType::event):
    {
        QString data = getData(msg);
        QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject root = doc.object();
        if(!root.keys().contains("name")){
            qDebug() << "Event does not contain name";
            return;
        }
        QString name = root.value("name").toString();
        if(!name.startsWith("model:")){
            qDebug() << "Unknown event name";
            qDebug() << data;
            return;
        }

        QString model = name.mid(6);

        parseEvent(model,root);

        return;
    }
    case(MessageType::ack):
    {
        //Id to the left of "+" and data to the right

        QString data = getData(msg);

        int ipluss = data.indexOf("+");
        if(ipluss<0){return;}

        QString before = data.mid(0,ipluss);

        bool ok = false;
        int id = before.toInt(&ok);//Get the msg id
        if(!ok){return;}


        //QString json = data.mid(ipluss+1,data.size()-ipluss+1);
        QString json = data.mid(ipluss+1);//From + to eol

        Message m(type);
        m.data = json;
        m.id = id;

        emit messageParsed(m);

        return;
    }
    case(MessageType::error):
        emit messageParsed(Message(type));
        return;
    case(MessageType::noop):
        emit messageParsed(Message(type));
        return;
    }

}
