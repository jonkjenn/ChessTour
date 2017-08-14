#ifndef CHESS24_H
#define CHESS24_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <optional>

struct UserData{
    bool result = false;
    QString authToken;
    QString uuid;
    QString permalink;
    QString country;
    QString name;
    bool isPremium=false;
    bool isRegistered=false;
};

class Chess24: public QObject
{
    Q_OBJECT
private:
    static const QString loginurl;
    QNetworkAccessManager &qnam;

    void httpFinished(QNetworkReply *, QString username, QString password);
    static std::optional<QString> getCSRF(QString source);
    static UserData getUserData(const QByteArray &data);
public:
    Chess24(QObject *parent,QNetworkAccessManager &);
    void login(QString username, QString password);
private slots:
    void redirected(const QUrl &);
    void gotError(QNetworkReply::NetworkError);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
signals:
    void loginResult(UserData);
};

#endif // CHESS24_H
