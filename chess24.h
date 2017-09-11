#ifndef CHESS24_H
#define CHESS24_H

#include "loginmanager.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <optional>


class Chess24: public LoginManager
{
    Q_OBJECT
private:
    static const QString loginurl;
    QNetworkAccessManager &qnam;

    void httpFinished(QNetworkReply *, QString username, QString password);
    static std::optional<QString> getCSRF(QString source);
    static UserData getUserData(const QByteArray &data);
    void downloadUserData(UserData::LoginSource onTryLogin = UserData::LoginSource::COOKIE);
public:
    Chess24(QObject *parent,QNetworkAccessManager &);
public slots:
    void onCheckLoggedIn(UserData::LoginSource source) override;
    void onTryLogin(QString username, QString password) override;
private slots:
    void redirected(const QUrl &);
    void gotError(QNetworkReply::NetworkError);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
};

#endif // CHESS24_H
