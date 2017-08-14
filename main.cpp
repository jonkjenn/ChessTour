#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "backend.h"
#include "tournamentviewmodel.h"
#include "tournamentsmodel.h"

static QJSValue backendSingleton(QQmlEngine *engine,QJSEngine *jsengine){
    QJSValue backend = engine->newQObject(new Backend(engine));
    return backend;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterSingletonType("jonkjenn.backend",1,0,"Backend",&backendSingleton);

    TournamentsModel *tm = new TournamentsModel(&app);
    TournamentViewModel *tvm = new TournamentViewModel(&app);
    tvm->setSourceModel(tm);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("tourModel",tvm);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
