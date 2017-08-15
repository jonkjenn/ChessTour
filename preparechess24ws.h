#ifndef PREPARECHESS24WS_H
#define PREPARECHESS24WS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class PrepareChess24WS:public QObject
{
    Q_OBJECT
public:
    PrepareChess24WS(QObject *parent, QNetworkAccessManager &qnam);
    void start();
private:
    QNetworkAccessManager &qnam;
    void notificationRespons(QNetworkReply *reply);
    QString notificationServer = "";
    QString wssId = "";
    QString parseWSSId(QString data);
    static QString parseNotficationServer(QString data);
    void wssidRespons(QNetworkReply *reply);
signals:
    void success(QString,QString);
    void failure(QString error);
};

#endif // PREPARECHESS24WS_H
