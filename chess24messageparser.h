#ifndef CHESS24MESSAGEPARSER_H
#define CHESS24MESSAGEPARSER_H
#include <QObject>
#include "message.h"

class Chess24MessageParser:public QObject
{
    Q_OBJECT
public:
    Chess24MessageParser(QObject *);


signals:
     void messageParsed(Message);

   public slots:
    void parseMessage(QString msg);
};

#endif // CHESS24MESSAGEPARSER_H
