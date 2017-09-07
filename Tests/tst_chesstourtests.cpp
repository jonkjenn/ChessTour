#include <QtTest>
#include <QCoreApplication>
#include <QSignalSpy>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>

// add necessary includes here

#include "../chess24messages.h"
#include "../chess24.h"
#include "../tokencontainer.h"
#include "../sqlhelper.h"

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

    void test_transformWebTournament();
    void test_getWebTournamentDiffMap();

    void test_sqlHelper();

    void test_tokenContainer();
    //void test_rootTournamentByName();

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

void ChessTourTests::test_transformWebTournament()
{
    using namespace Chess24Messages;

    QVERIFY(TestData::tourDetails.at(0) == "{");
    QVERIFY(TestData::tourDetails.at(TestData::tourDetails.size()-1) == "}");
    QJsonDocument doc = QJsonDocument::fromJson(TestData::tourDetails.toUtf8());
    QVERIFY(doc.isObject());
    QVariant test = doc.toVariant();
    QVariantMap map = test.toMap();

    QVariantMap result = transformWebTournament(map);

    QVERIFY(result.contains("players"));
    QVERIFY(result.value("players").type() == QVariant::Type::Map);
    QVariantMap players = result.value("players").toMap();

    QVERIFY(players.value("Name").toList().contains("Navara, David"));
    QVERIFY(players.value("FideId").toList().contains("309095"));
    QVERIFY(players.value("Name").toList().indexOf("Navara, David")==players.value("FideId").toList().indexOf("309095"));

    QVERIFY(players.value("Name").toList().contains("Le, Quang Liem"));
    QVERIFY(players.value("FideId").toList().contains("12401137"));
    QVERIFY(players.value("Name").toList().indexOf("Le, Quang Liem")==players.value("FideId").toList().indexOf("12401137"));

    QVERIFY(result.value("Title").toString() == "St. Louis Rapid & Blitz GCT");
    QVERIFY(result.value("Description").toString() == "Garry Kasparov returns as a wild card for the Saint Louis Rapid and Blitz, the penultimate stage of the 2017 Grand Chess Tour. The 10-player tournament takes place in the Chess Club and Scholastic Center of Saint Louis from 14-19 August and sees tour regulars Aronian, Caruana, Nakamura, Anand, Karjakin and Nepomniachtchi take on wild cards Dominguez, Navara, Le Quang Liem and of course Kasparov. The prize fund is $150,000, with $37,500 for 1st place.\nThe rapid section is a single round-robin with 3 rounds each day on the first 3 days at a time control of 25 minutes for all moves and a 10-second delay from move 1. The final 2 days are a blitz double round-robin, with 18 rounds of 5 minutes + 3-second delay blitz. Rapid games count double, with 2 points for a win and 1 for a draw. Official website: <a target=\"_blank\" href=\"http://grandchesstour.org/2017-grand-chess-tour/stlrb\">grandchesstour.org</a>");
    QVERIFY(result.value("Name").toString() == "grand-chess-tour-saint-louis-rapid-and-blitz-2017");

    QVERIFY(result.value("roundWrapper").type() == QVariant::Type::Map);
    QVariantMap rounds = result.value("roundWrapper").toMap();
    QVERIFY(rounds.value("Number").toList().contains(1));
    QVERIFY(rounds.value("Number").toList().contains(6));

    QVariantMap roundList = rounds.value("rounds").toMap();
    QVariantMap round = roundList.value(roundList.keys().first()).toMap();
    QCOMPARE(round.value("StartDate").toLongLong(),static_cast<qlonglong>(1502733600000));
    QCOMPARE(round.value("Number").toInt(),1);
    QVariantMap round10;
    for(auto ll:roundList.keys()){
        QVariantMap l = roundList.value(ll).toMap();
        if(l.value("Number") == 10){
            QCOMPARE(l.value("StartDate").toLongLong(),static_cast<qlonglong>(1502992800000));
            round10 = l;
            break;
        }
    }

    QVariantMap round10Matches = round10.value("matchWrapper").toMap();

    QCOMPARE(round10Matches.value("games").toList().size(),5);

    QVariantMap r;
    for(auto m:round10Matches.value("games").toList()){
        r = m.toMap();
        if(r.value("Number") == 1 && r.value("GameNumber") == 5){
            break;
        }
    }

    QCOMPARE(r.value("Number").toInt(),1);
    QCOMPARE(r.value("GameNumber").toInt(),5);
    QVERIFY(r.value("WhiteFide") =="3503240");
    QVERIFY(r.value("BlackFide") =="309095");
    QVERIFY(r.value("Status") == "upcoming");
    QVERIFY(r.value("EngineScore") == 23);

    QVariantList round0Matches = round.value("matchWrapper").toMap().value("games").toList();
    QVERIFY(round0Matches.size()>0);
    for(auto m:round0Matches){
        r = m.toMap();
        if(r.value("Number") == 1 && r.value("GameNumber") == 1){
            break;
        }
    }

    QVERIFY(r.value("WhiteFide") =="4100018");
    QVERIFY(r.value("BlackFide") =="14109603");
    QVERIFY(r.value("ResultWhite") == 0.5);
    QVERIFY(r.value("ResultBlack") == 0.5);
    QVERIFY(r.value("Status") == "finished");
    QVERIFY(r.value("EngineScore") == 0);
    QVERIFY(r.value("CurrentFEN") == "3r4/8/2p1Bpk1/1p6/p2P1pPP/P7/1Pb2K2/4R3 w - - 7 45");
    QVERIFY(r.value("EarlierFEN") =="3r4/8/2p1Bpk1/1p6/p2P1pPP/P7/1Pb2K2/4R3 w - - 3 43");
}

void ChessTourTests::test_getWebTournamentDiffMap()
{
    QJsonDocument doc = QJsonDocument::fromJson(TestData::webTournamentDiff.toUtf8());
    QVariantMap map = doc.toVariant().toMap();

    QVariantMap result = Chess24Messages::transformWebTournament(map.value("diffs").toMap());

    QCOMPARE(result.value("roundWrapper").toMap().value("Number").toList().at(0).toInt(),9);
    QVariantMap rounds = result.value("roundWrapper").toMap().value("rounds").toMap();
    QCOMPARE(rounds.size(),1);

    QVariantMap round = rounds.value(rounds.keys().at(0)).toMap();
    QVariantMap matchWrapper = round.value("matchWrapper").toMap();
    QVariantList games = matchWrapper.value("games").toList();
    QVariantList Number = matchWrapper.value("Number").toList();
    QVariantList GameNumber = matchWrapper.value("GameNumber").toList();

    QCOMPARE(games.size(),2);
    QCOMPARE(Number.size(),2);
    QCOMPARE(GameNumber.size(),2);
    QVariantMap mm5;
    QVariantMap mmX;
    for(auto m:games){
        auto mm= m.toMap();
        if(mm.value("Number") == 1 && mm.value("GameNumber") == 5){
            mm5 = mm;
        }
        else{
            mmX = mm;
        }
    }
    QCOMPARE(mm5.value("EngineScore").toInt(),147);
    QVERIFY(mm5.value("CurrentFEN")=="8/5k1p/pp3p2/5K2/PP4PP/8/8/8 w - - 7 60");
    QVERIFY(mm5.value("EarlierFEN") == "8/7p/pp2kp2/8/PP2K1PP/8/8/8 w - - 3 58");
    QVERIFY(mmX.value("CurrentFEN")=="8/3r3k/3P2p1/2Q4p/p7/7P/4q1P1/3R3K w - - 2 57");
    QVERIFY(mmX.value("EarlierFEN")=="8/3r3k/3Pq1p1/p6p/3Q4/7P/6PK/3R4 w - - 0 55");

}

//TODO: split into separate tests
void ChessTourTests::test_sqlHelper()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(QDir::currentPath() + QDir::separator() + "test.sqlite");
    db.setDatabaseName(":memory:");
    db.open();
    db.transaction();
    QSqlQuery qdrop("drop table Test1",db);
    qdrop.finish();
    QSqlQuery qdrop2("drop table Test2",db);
    qdrop2.finish();
    QSqlQuery q("CREATE TABLE Test1 (Id INTEGER PRIMARY KEY AUTOINCREMENT, Value1 TEXT NOT NULL, Value2 STRING NOT NULL);",db);
    q.finish();
    QSqlQuery q2("CREATE TABLE Test2 (Id INTEGER PRIMARY KEY AUTOINCREMENT, Test1Id INTEGER REFERENCES Test1 (Id) ON DELETE CASCADE, Value1 STRING NOT NULL, Value2 INTEGER NOT NULL);",db);
    q2.finish();
    db.commit();

    QVariantList var1 = {"lorem", "ipsum", "dolor", "sit", "amet"};
    QVariantList var2 = {1, 567, 77, 5, 14};
    int rows = SqlHelper::insertLists(db,"Test1",{var1,var2},{"Value1", "Value2"});
    //QCOMPARE(rows,5);

    QSqlQuery t1("select * from Test1 order by Id asc;",db);
    int i=1;
    while(t1.next()){
        QCOMPARE(t1.value(0).toInt(),i);
        QVERIFY(t1.value(1) == var1.at(i-1));
        QVERIFY(t1.value(2) == var2.at(i-1));
        ++i;
    }
    QCOMPARE(i,6);
    QVariantList pks = {1,2,3,4,5};
    var1 =  {"timeam","electram","te","eum", "dolore"};
    var2 = {4283238, 1239393, 1, 2, -55858};
    rows = SqlHelper::insertLists(db,"Test2",{var1,var2,pks},{"Value1", "Value2", "Test1Id"});

    QSqlQuery t2("select * from Test2 order by Id asc;",db);
    i=1;
    while(t2.next()){
        QCOMPARE(t2.value(0).toInt(),i);
        QVERIFY(t2.value(1) == pks.at(i-1));
        QVERIFY(t2.value(2) == var1.at(i-1));
        QVERIFY(t2.value(3) == var2.at(i-1));
        ++i;
    }
    QCOMPARE(i,6);

    QVariantMap whereList;
    whereList.insert("Test1Id",pks);
    QVariantMap whereValues;
    QVariantList res = SqlHelper::getColumnList(db,"Test2","Id",5,whereList,whereValues);
    QCOMPARE(res.size(),5);
    QVERIFY(res.contains(1));
    QVERIFY(res.contains(2));
    QVERIFY(res.contains(3));
    QVERIFY(res.contains(4));
    QVERIFY(res.contains(5));

    whereValues.insert("Value1","eum");
    QVariantList res2 = SqlHelper::getColumnList(db,"Test2","Id",5,whereList,whereValues);
    QCOMPARE(res2.value(0).toInt(),4);

    QVariantMap map;
    map.insert("Value1","test");
    map.insert("Value2",999);
    QVariantMap where;
    where.insert("Test1Id",5);
    SqlHelper::updateTable(db,"Test2",map,where);

    QSqlQuery qs("select * from Test2 where Test1Id = 5");
    qs.next();
    QCOMPARE(qs.value(0).toInt() , 5);
    QCOMPARE(qs.value(1).toInt(), 5);
    QVERIFY(qs.value(2) == "test");
    QCOMPARE(qs.value(3).toInt(),999);

    QVariantMap map2;
    map2.insert("Value1",12349595.4);
    map2.insert("Value2","1");
    SqlHelper::updateTable(db,"Test2",map2,where);

    QSqlQuery qs2("select * from Test2 where Test1Id = 5");
    qs2.next();
    QCOMPARE(qs2.value(0).toInt() , 5);
    QCOMPARE(qs2.value(1).toInt(), 5);
    QVERIFY(qs2.value(2) == "12349595.4");
    QCOMPARE(qs2.value(3).toInt(),1);



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

void ChessTourTests::test_tokenContainer(){
    QTimer timer(nullptr);
    timer.setInterval(0);
    timer.setSingleShot(true);
    TokenContainer t(nullptr,timer,5);
    QSignalSpy spy(&t,SIGNAL(notifyTokenAvailableChanged(bool)));

    QVERIFY(t.getToken() == false);
    QVERIFY(t.getToken() == false);
    QVERIFY(t.getToken() == false);
    timer.start();
    QCoreApplication::processEvents();
    QCOMPARE(spy.count() , 1);
    QVERIFY(t.getToken() == true);
    QCOMPARE(spy.count() , 2);
    QVERIFY(t.getToken() == false);
    timer.start();
    QCoreApplication::processEvents();
    QCOMPARE(spy.count() , 3);
    QVERIFY(t.getToken() == true);
    QCOMPARE(spy.count() , 4);
    QVERIFY(t.getToken() == false);
    timer.start();
    QCoreApplication::processEvents();
    timer.start();
    QCoreApplication::processEvents();
    timer.start();
    QCoreApplication::processEvents();
    timer.start();
    QCoreApplication::processEvents();
    timer.start();
    QCoreApplication::processEvents();
    QCOMPARE(spy.count() , 5);
    QVERIFY(t.getToken() == true);//1
    QVERIFY(t.getToken() == true);//2
    QVERIFY(t.getToken() == true);//3
    QVERIFY(t.getToken() == true);//4
    QVERIFY(t.getToken() == true);//5
    QCOMPARE(spy.count() , 6);
    QVERIFY(t.getToken() == false);//5

    TokenContainer t2(nullptr,timer,1);
    QSignalSpy spy2(&t2,SIGNAL(notifyTokenAvailableChanged(bool)));
    QVERIFY(t2.getToken() == false);
    QVERIFY(t2.getToken() == false);
    timer.start();
    QCoreApplication::processEvents();
    QCOMPARE(spy2.count() , 1);
    QVERIFY(t2.getToken() == true);
    QCOMPARE(spy2.count() , 2);
    QVERIFY(t2.getToken() == false);
    timer.start();
    QCoreApplication::processEvents();
    timer.start();
    QCoreApplication::processEvents();
    QCOMPARE(spy2.count() , 3);
    QVERIFY(t2.getToken() == true);
    QCOMPARE(spy2.count() , 4);
    QVERIFY(t2.getToken() == false);
}

QTEST_MAIN(ChessTourTests)

/*int main(int argc, char *argv[]){
    ChessTourTests test;
    test.test_sqlHelper();
}*/


#include "tst_chesstourtests.moc"
