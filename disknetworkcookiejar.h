#ifndef DISKNETWORKCOOKIEJAR_H
#define DISKNETWORKCOOKIEJAR_H
#include <QNetworkCookieJar>

class DiskNetworkCookieJar:public QNetworkCookieJar
{
public:
    DiskNetworkCookieJar(QObject *);

    // QNetworkCookieJar interface
public:
    void saveCookieJar();
    QList<QNetworkCookie> loadCookies();
};

#endif // DISKNETWORKCOOKIEJAR_H
