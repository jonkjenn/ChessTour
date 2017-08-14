#include "backend.h"
#include "tournamentsmodel.h"
#include "PGNGameCollection.h"
#include <sstream>
#include <iostream>
#include "tournamentviewmodel.h"
#include <QQmlApplicationEngine>
#include "chess24.h"
#include <QQmlContext>

BackEnd::BackEnd(QObject *parent):QObject(parent)
{
    /*//Chess24 c24;
    QUrl pgnurl("http://theweekinchess.com/assets/files/pgn/chgbr17.pgn");

    std::cout << "Getting pgn" << std::endl;
    QNetworkRequest request(pgnurl);
    request.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla Firefox");
    reply = qnam.get(request);
    connect(reply,&QNetworkReply::finished,this,&BackEnd::httpFinished);**/

    TournamentsModel *tm = new TournamentsModel(this);
    TournamentViewModel *tvm = new TournamentViewModel(this);
    tvm->setSourceModel(tm);

    engine.rootContext()->setContextProperty("tourModel",tvm);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    /*if (engine.rootObjects().isEmpty())
        return -1;*/

    c24.login("test","test");
    connect(&c24,&Chess24::loginResult,this,&BackEnd::loggedIn);
}

void BackEnd::httpFinished()
{
    std::cout << "Got data" << std::endl;
    QByteArray data = reply->readAll();
    QString txt(data);

    pgn::GameCollection games;

    std::stringstream ss;
    ss << txt.toStdString();
    ss >> games;

    if(games.size()>0){
        pgn::Game g = games[0];
        std::cout << g.moves() << std::endl;
    }
}

void BackEnd::loggedIn(UserData data)
{
    if(data.result){
        qDebug() << "Logged in";
    }else{
        qDebug() << "Login failed";
        //TODO: Retry login gui
    }
}
