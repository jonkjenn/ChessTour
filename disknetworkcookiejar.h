#ifndef DISKNETWORKCOOKIEJAR_H
#define DISKNETWORKCOOKIEJAR_H
#include <QNetworkCookieJar>

class DiskNetworkCookieJar:public QNetworkCookieJar
{
public:
    DiskNetworkCookieJar();

    // QNetworkCookieJar interface
public:
    static void loadCookieJar(QNetworkCookieJar &jar);
    void saveCookieJar();
};

#endif // DISKNETWORKCOOKIEJAR_H
