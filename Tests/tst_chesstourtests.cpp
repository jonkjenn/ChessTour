#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

#include "../tournamentsmodel.h"

class ChessTourTests : public QObject
{
    Q_OBJECT

public:
    ChessTourTests();
    ~ChessTourTests();

private slots:
    void test_case1();

};

ChessTourTests::ChessTourTests()
{

}

ChessTourTests::~ChessTourTests()
{

}

void ChessTourTests::test_case1()
{
    TournamentsModel tm;
}

QTEST_MAIN(ChessTourTests)

#include "tst_chesstourtests.moc"
