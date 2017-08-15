#ifndef DISKNETWORKCOOKIEJAR_H
#define DISKNETWORKCOOKIEJAR_H
#include <QNetworkCookieJar>

class DiskNetworkCookieJar:public QNetworkCookieJar
{
public:
    DiskNetworkCookieJar();

    // QNetworkCookieJar interface
public:
    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) override;
    bool insertCookie(const QNetworkCookie &cookie) override;
    bool deleteCookie(const QNetworkCookie &cookie) override;
    static void loadCookieJar(QNetworkCookieJar &jar);
    void saveCookieJar();
};

#endif // DISKNETWORKCOOKIEJAR_H
