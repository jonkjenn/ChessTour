#ifndef CHESS24WEBSOCKET_H
#define CHESS24WEBSOCKET_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QString>
#include <QSslError>
#include <QAbstractSocket>
#include <QMap>

#include <QQueue>
#include <QTimer>
#include <memory>

#include "wsrequest.h"
#include "chess24.h"

class PrepareChess24WS;
class Chess24Login;

class QNetworkAccessManager;
class QNetworkReply;
class Message;

class Chess24Websocket: public QObject
{
    Q_OBJECT
public:
    Chess24Websocket(QObject *,const QNetworkAccessManager &,const Chess24Login &, PrepareChess24WS &);
    void connectWS(QString notificationServer, QString wssId);
    bool isConnected();

     enum class ErrorReason{
         transport_not_supported = 0,
         client_not_handshaken = 1,
         unauthorized = 2
     };

     enum class ErrorAdvice{
             reconnect = 0
     };

     void handleMessage(Message msg);

     WSRequest *sendMessage(QString msg, int messageId);
     void sendMessage(QString msg);

     int increaseAndGetMessageId();
     void send();
signals:
     void connected();
     void messageReceived(QString);
public slots:
     void onLoggedInChanged();

private:
     QTimer tokenTimer;
     QTimer msgQTimer;
     int maxTokens = 2;
     int tokens = 2;
     QQueue<QString> msgQ;
     bool loggedIn = false;

    int messageId();
    const QNetworkAccessManager &qnam;
    UserData userData;
    QMap<int,WSRequest*> requests;

    const Chess24Login &chess24Login;
    PrepareChess24WS &prepWS;

    QWebSocket ws;
    int m_messageId = 1;

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
