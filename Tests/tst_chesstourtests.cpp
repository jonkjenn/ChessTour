#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

#include "../tournamentsmodel.h"
#include "../chess24messages.h"
#include "../chess24.h"

#include "testdata.h"

#include <QJsonDocument>
#include <QJsonObject>

class ChessTourTests : public QObject
{
    Q_OBJECT

public:
    ChessTourTests();
    ~ChessTourTests();

private slots:
    void test_case1();


    //chess24messages
    //These tests depends on that the receiver parses the json the same way
    void test_connect();
    void test_getTournamentIds();
    void test_getBroadcastGame();
    void test_subscribeTournament();
    void test_subscribeBrodcastGame();

    void test_rootTournamentByName();

    void test_getData();
    void test_getHeader();

};

ChessTourTests::ChessTourTests()
{
}

ChessTourTests::~ChessTourTests()
{

}

void ChessTourTests::test_case1()
{
}

void ChessTourTests::test_connect()
{
    //Wrong approach to testing this, should be independent of the order of the elements.

    using namespace Chess24Messages;
    using namespace Chess24Messages::Helpers;
    UserData data;
    data.authToken = "testAuthToken";
    data.isPremium = false;
    data.isRegistered = true;
    data.name = "Tester";

    QString msg = Chess24Messages::connect(data,1);

    QVERIFY(getHeader(msg) == "5:1+::");

    QJsonDocument doc = QJsonDocument::fromJson(getData(msg).toUtf8());
    QJsonObject o1 = doc.object();
    QVERIFY(o1.size() == 2);
    QVERIFY(o1.keys().contains("name"));
    QVERIFY(o1["name"].toString()  == "connect");
    QVERIFY(o1.keys().contains("args"));
    QVERIFY(o1["args"].isArray());
    QVERIFY(o1["args"].toArray()[0].isString());
    QVERIFY(o1["args"].toArray()[0] == "testAuthToken");
    QVERIFY(o1["args"].toArray()[1].isObject());
    QVERIFY(o1["args"].toArray()[1].toObject().keys().contains("isGuest"));
    QVERIFY(o1["args"].toArray()[1].toObject().keys().contains("isPremium"));
    QVERIFY(o1["args"].toArray()[1].toObject().keys().contains("isAdmin"));
    QVERIFY(o1["args"].toArray()[1].toObject().keys().contains("userName"));
    QVERIFY(o1["args"].toArray()[1].toObject()["userName"] == "Tester");
}

void ChessTourTests::test_getTournamentIds()
{
    using namespace Chess24Messages;
    using namespace Chess24Messages::Helpers;
    QString msg = Chess24Messages::getTouramentIds(2);

    QVERIFY(getHeader(msg) == "5:2+::");

    QJsonDocument doc = QJsonDocument::fromJson(getData(msg).toUtf8());
    QJsonObject o1 = doc.object();

    QVERIFY(o1.keys().contains("name"));
    QVERIFY(o1.keys().contains("args"));
    QVERIFY(o1["name"].toString() == "getTournamentIds");
    QVERIFY(o1["args"].isArray());
    QVERIFY(o1["args"].toArray()[0] == "web");

}

void ChessTourTests::test_getBroadcastGame()
{
    using namespace Chess24Messages;
    using namespace Chess24Messages::Helpers;
    QString msg = Chess24Messages::getBroadcastChessGame(2,"test");

    QVERIFY(getHeader(msg) == "5:2+::");

    QJsonDocument doc = QJsonDocument::fromJson(getData(msg).toUtf8());
    QJsonObject o1 = doc.object();

    QVERIFY(o1.keys().contains("name"));
    QVERIFY(o1.keys().contains("args"));
    QVERIFY(o1["name"].toString() == "get");
    QVERIFY(o1["args"].isArray());
    QVERIFY(o1["args"].toArray()[0].toString() == "model:BroadcastChessGameRedisAR:test");

}

void ChessTourTests::test_subscribeTournament()
{
    using namespace Chess24Messages;
    using namespace Chess24Messages::Helpers;

    QString msg = Chess24Messages::subscribeWebTournament("test321");

    QJsonDocument doc = QJsonDocument::fromJson(getData(msg).toUtf8());
    QJsonObject o1 = doc.object();

    QVERIFY(o1.keys().contains("name"));
    QVERIFY(o1.keys().contains("args"));
    QVERIFY(o1["args"].toArray().size() == 1);
    QVERIFY(o1["args"].toArray()[0].toString() == "model:webTournamentRedisAR:test321");
}

void ChessTourTests::test_subscribeBrodcastGame()
{
    using namespace Chess24Messages;
    using namespace Chess24Messages::Helpers;

    QString msg = Chess24Messages::subscribeBroadcastChessGame("test321");

    QJsonDocument doc = QJsonDocument::fromJson(getData(msg).toUtf8());
    QJsonObject o1 = doc.object();

    QVERIFY(o1.keys().contains("name"));
    QVERIFY(o1.keys().contains("args"));
    QVERIFY(o1["args"].toArray().size() == 1);
    QVERIFY(o1["args"].toArray()[0].toString() == "model:BroadcastChessGameRedisAR:test321");
}

void ChessTourTests::test_rootTournamentByName()
{
    RootItem root(nullptr);
    Tournament tour(nullptr,"Test tournament");
    bool added = root.addChild(&tour);
    QVERIFY(added == true);
    const Tournament *t = &tour;
    const Tournament *rt = root.item("Test tournament");
    QVERIFY(rt == t);
    root.addChild(new Tournament(nullptr,"Test tournament2"));
    root.addChild(new Tournament(nullptr,"Test tournament3"));
    root.addChild(new Tournament(nullptr,"Test tournament4"));
    root.addChild(new Tournament(nullptr,"Test tournament5"));
    root.addChild(new Tournament(nullptr,""));
    root.addChild(new Tournament(nullptr,"asqiweqwdwqn)"));
    root.addChild(new Tournament(nullptr,"asqiweqwdwqn)"));
    root.addChild(new Tournament(nullptr,"___)"));
    root.addChild(new Tournament(nullptr,"  "));
    rt = root.item("Test tournament");
    QVERIFY(rt == t);
    rt = root.item("Test tournament5");
    QVERIFY(rt != t);
    QVERIFY(rt == root.tournamentChild(4));
}

void ChessTourTests::test_getData()
{
    using namespace Chess24Messages;
    using namespace Chess24Messages::Helpers;

    QVERIFY(getData("5:1+::{}") == "{}");
    QVERIFY(getData("5:::{}") == "{}");
    QVERIFY(getData("5:3+::{data}") == "{data}");
    QVERIFY(getData("5:3+:55:{data}") == "{data}");
}

void ChessTourTests::test_getHeader()
{
    using namespace Chess24Messages;
    using namespace Chess24Messages::Helpers;

    QVERIFY(getHeader("5:1+::{}") == "5:1+::");
    QVERIFY(getHeader("5:::{}") == "5:::");
    QVERIFY(getHeader("5:1+::{asdkjaskdjaskdjaskdjkasjdkasjdkasj}") == "5:1+::");
    QVERIFY(getHeader("6:asdkasdjsakd:asdasd:{}") == "6:asdkasdjsakd:asdasd:");
}

QTEST_MAIN(ChessTourTests)

#include "tst_chesstourtests.moc"
