#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tournamentsmodel.h"
#include "tournamentviewmodel.h"
#include "chess24.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    TournamentsModel *tm = new TournamentsModel(&app);
    TournamentViewModel *tvm = new TournamentViewModel(&app);
    tvm->setSourceModel(tm);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("tourModel",tvm);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    Chess24 c24;



    return app.exec();
}
