#ifndef CHESS24MANAGER_H
#define CHESS24MANAGER_H

#include <QObject>
#include <QVariant>
#include <QJsonObject>

#include <functional>

class Chess24Websocket;
class WSRequest;

class Chess24Manager: public QObject
{
    Q_OBJECT
public:
    Chess24Manager(QObject *, Chess24Websocket &c24ws);
    void getTournaments();
    void subscribeTournament(QString name);
    void getTournament(QString name);
private:
    Chess24Websocket &c24ws;
    WSRequest *sendMessage(std::function<QString (int)> msg);
    WSRequest *sendMessage(std::function<QString (int, QString)> msgFunc, QString msgData);

signals:
    void gotTournaments(QVariantList names);
    void gotTournamentDetails(QJsonObject details);

};

#endif // CHESS24MANAGER_H
