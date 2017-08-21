#ifndef WSREQUEST_H
#define WSREQUEST_H

#include <QObject>

class WSRequest:public QObject
{
    Q_OBJECT
public:
    WSRequest(int id, QObject *parent);
    const int id;
    void data(QString);
signals:
    void finished(QString);
};

#endif // WSREQUEST_H
