#ifndef USERDATA_H
#define USERDATA_H
#include <QString>
#include <QMetaType>


struct UserData{
    enum LoginSource{USERPASS,COOKIE};
    bool result = false;
    QString authToken;
    QString uuid;
    QString permalink;
    QString country;
    QString name;
    bool isPremium=false;
    bool isRegistered=false;
    LoginSource loginSource;
};

//We want to be able to use these in signals
Q_DECLARE_METATYPE(UserData)
Q_DECLARE_METATYPE(UserData::LoginSource)

#endif // USERDATA_H
