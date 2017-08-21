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
    file.open("cookies.jar");//Content is overwritten

    //TODO: error handling

    //Save each cookie as a line in the file
    for(auto c:allCookies()){
        file << c.toRawForm().toStdString() << "\n";
    }

    file.close();
}

void DiskNetworkCookieJar::loadCookieJar(QNetworkCookieJar &jar){
    ifstream file;
    file.open("cookies.jar");
    //TODO:: error handling
    string line;
    while(getline(file,line)){

        //Empty if parsing error
        QList<QNetworkCookie> cookies = QNetworkCookie::parseCookies(QByteArray::fromStdString(line));
        for(auto c:cookies){
            jar.insertCookie(c);
        }
    }
    file.close();
}
