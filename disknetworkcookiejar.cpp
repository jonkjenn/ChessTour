#include "disknetworkcookiejar.h"
#include <iostream>
#include <fstream>
#include <QNetworkCookie>

using namespace std;

DiskNetworkCookieJar::DiskNetworkCookieJar()
{

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
