#include "disknetworkcookiejar.h"
#include <iostream>
#include <fstream>
#include <QNetworkCookie>

using namespace std;

DiskNetworkCookieJar::DiskNetworkCookieJar()
{

}

bool DiskNetworkCookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url)
{
    return QNetworkCookieJar::setCookiesFromUrl(cookieList,url);
}

bool DiskNetworkCookieJar::insertCookie(const QNetworkCookie &cookie)
{
    return QNetworkCookieJar::insertCookie(cookie);
}

bool DiskNetworkCookieJar::deleteCookie(const QNetworkCookie &cookie)
{
    return QNetworkCookieJar::deleteCookie(cookie);
}

void DiskNetworkCookieJar::saveCookieJar(){
    ofstream file;
    file.open("cookies.jar");

    for(auto c:allCookies()){
        file << c.toRawForm().toStdString() << "\n";
    }

    file.close();
}

void DiskNetworkCookieJar::loadCookieJar(QNetworkCookieJar &jar){
    ifstream file;
    file.open("cookies.jar");
    string line;
    while(getline(file,line)){
        QList<QNetworkCookie> cookies = QNetworkCookie::parseCookies(QByteArray::fromStdString(line));
        for(auto c:cookies){
            jar.insertCookie(c);
        }
    }

}
