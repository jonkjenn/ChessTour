#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "backend.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    BackEnd b(&app);

    return app.exec();
}
