#include "disknetworkcookiejar.h"
#include <iostream>
#include <fstream>
#include <QNetworkCookie>

using namespace std;

DiskNetworkCookieJar::DiskNetworkCookieJar(QObject *parent):
    QNetworkCookieJar(parent)
{
    QList<QNetworkCookie> cookies = loadCookies();
    for(auto c:cookies){
        insertCookie(c);
    }
}

void DiskNetworkCookieJar::saveCookieJar(){
    ofstream file;
    file.open("cookies.jar");//Content is overwritten

    //TODO: error handling

    //Save each cookie as a line in the file
    for(auto c:allCookies()){
        file << c.toRawForm().toStdString() << "\n";
    }

    file.close();
}


QList<QNetworkCookie> DiskNetworkCookieJar::loadCookies(){
    ifstream file;
    file.open("cookies.jar");
    //TODO:: error handling
    string line;
    QList<QNetworkCookie> cookies;
    while(getline(file,line)){

        //Empty if parsing error
        cookies.append(QNetworkCookie::parseCookies(QByteArray::fromStdString(line)));
    }
    file.close();
    return cookies;
}
