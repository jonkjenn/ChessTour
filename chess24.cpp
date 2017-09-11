#include "chess24.h"

#include <QtNetwork>
#include <QObject>
#include <QJsonDocument>

#include <optional>
#include <iostream>

#include "disknetworkcookiejar.h"

const QString Chess24::loginurl = "https://chess24.com/en/login";

///
/// \brief Chess24::Chess24
/// \param parent
/// \param qnam
///
/// Fasilitates interactions with the Chess24 website
///
Chess24::Chess24(QObject *parent,QNetworkAccessManager &qnam):LoginManager(parent),qnam(qnam)
{
}

void Chess24::onTryLogin(QString username, QString password)
{
    //Load the login page so we can get the CSRF code to perform login
    QNetworkReply *reply = qnam.get(QNetworkRequest(Chess24::loginurl));
    connect(reply,&QNetworkReply::finished,[this,reply,username,password](){httpFinished(reply,username,password); });
    //connect(&qnam,&QNetworkAccessManager::sslErrors,this,&Chess24::sslErrors);
}

void Chess24::onCheckLoggedIn(UserData::LoginSource source)
{
    downloadUserData(source);
}

void Chess24::httpFinished(QNetworkReply *reply, QString username, QString password)
{
    QByteArray data = reply->readAll();
    QString txt{data};
    std::optional<QString> csrf = getCSRF(txt);

    if(!csrf){
        UserData ud;
        ud.result = false;
       emit loginResult(ud);
        return;
    }

    QUrlQuery qu;
    qu.addQueryItem("LoginForm[emailOrUsername]",username);
    qu.addQueryItem("LoginForm[password]",password);
    qu.addQueryItem("csrf",csrf.value());
    qu.addQueryItem("LoginForm[rememberMe]","0");
    qu.addQueryItem("LoginForm[rememberMe]","1");
    qu.addQueryItem("yt1","Login");

    QNetworkRequest req(QUrl("https://chess24.com/en/login"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    req.setRawHeader(QByteArray("origin"),QByteArray("https://chess24.com"));
    req.setRawHeader(QByteArray("Referer"),QByteArray("https://chess24.com/en/login"));
    req.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36");

    qDebug() << username << " "<< password << " " << csrf.value();

    //Send login post
    QNetworkReply * rep = qnam.post(req,qu.query(QUrl::FullyEncoded).toUtf8());
    connect(rep,&QNetworkReply::finished,[this,rep](){
        //Check if logged in by using this api call
        downloadUserData(UserData::LoginSource::USERPASS);
        rep->deleteLater();
    });
    connect(rep,&QNetworkReply::redirected,this,&Chess24::redirected);
    connect(rep,static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),this,&Chess24::gotError);
}

void Chess24::downloadUserData(UserData::LoginSource source){

        QNetworkRequest req(QUrl("https://chess24.com/api/web/UserAPI/getUserData"));
        QNetworkReply *urep = qnam.get(req);
        connect(urep,&QNetworkReply::finished,[this,urep,source]{
            QByteArray data = urep->readAll();
            qDebug() << data;
            UserData udat = getUserData(data);
            udat.loginSource = source;

            if(udat.loginSource == UserData::LoginSource::USERPASS){
                DiskNetworkCookieJar *jar = static_cast<DiskNetworkCookieJar*>(qnam.cookieJar());
                jar->saveCookieJar();
            }

            emit loginResult(udat);
            urep->deleteLater();
        });
}

UserData Chess24::getUserData(const QByteArray &data){
    UserData d;
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonDocument ujdoc(QJsonDocument::fromJson((jdoc.object())["data"].toObject()["userData"].toObject()["value"].toString().toUtf8()));
    QJsonObject userdata = ujdoc.object();
    QJsonObject user = userdata["user"].toObject();
    d.authToken = user["authToken"].toString();
    d.country = user["country"].toString();
    d.name = user["name"].toString();
    d.permalink = user["permalink"].toString();
    d.uuid = user["uuid"].toString();
    QJsonObject level = userdata["level"].toObject();
    d.isRegistered = level["isRegistered"].toBool();
    d.isPremium = level["isPremium"].toBool();
    d.result = true;
    return d;
}

void Chess24::redirected(const QUrl &url)
{
    std::cout << url.toString().toStdString() << std::endl;

}

void Chess24::gotError(QNetworkReply::NetworkError error)
{
    qDebug() << error;
}

void Chess24::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{

    for(auto e:errors){
        qDebug() << e.errorString();
    }

}

///
/// \brief Chess24::getCSRF
/// \param source Source code of the login page
/// \return
///
///
/// Typical <input value="xxxxxxxxxxxxxx" name="csrf"/>
///
///
std::optional<QString> Chess24::getCSRF(QString source){
    int csrfidx = source.indexOf("csrf"); //The name attribute that says "csrf", is after the value attribute with the csrf code

    if(csrfidx<0){
        return {};
    }

    //Find the start of the code
    int start1 = source.lastIndexOf("value=",csrfidx-source.length());

    if(csrfidx<0){
        return {};
    }

    //Add number of letters in value="
    int start = start1 + 7;

    //Find the end quote
    int end = source.indexOf("\"",start);

    if(csrfidx<0){
        return {};
    }

    return source.mid(start,end-start);
}
