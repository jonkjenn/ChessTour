#ifndef CHESS24WEBSOCKET_H
#define CHESS24WEBSOCKET_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QString>
#include <QSslError>
#include <QAbstractSocket>
#include <QMap>

#include <memory>

#include "wsrequest.h"
#include "chess24.h"

class QNetworkAccessManager;
class QNetworkReply;

class Chess24Websocket: public QObject
{
    Q_OBJECT
public:
    Chess24Websocket(QObject *,QNetworkAccessManager &);
    void connectWS(UserData data, QString notificationServer, QString wssId);
    bool isConnected();

    WSRequest *getTournamentIds();


     enum MessageType{
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

     enum ErrorReason{
         transport_not_supported = 0,
         client_not_handshaken = 1,
         unauthorized = 2
     };

     enum ErrorAdvice{
             reconnect = 0
     };

signals:
     void connected();
private:
    QNetworkAccessManager &qnam;
    UserData userData;
    QMap<int,WSRequest*> requests;
    WSRequest* createRequest(QString msg);

    QWebSocket ws;
    int message_id = 1;

    bool m_isConnected = false;
    void setIsConnected(bool);

    void notificationRespons(QNetworkReply *);
    void wssidRespons(QNetworkReply *);
    static QString parseNotficationServer(QString);
    static QString parseWSSId(QString);

private slots:
    void onConnected();
    void onDisconnected();
    void onTextReceived(QString);
    void sslErrors(const QList<QSslError>);
    void binaryMessageReceived(const QByteArray);
    void socketError(QAbstractSocket::SocketError error);

};

#endif // CHESS24WEBSOCKET_H
