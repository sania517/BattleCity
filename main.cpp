#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "battlefieldobjects.h"
#include <QQmlContext>
#include "controller.h"
#include "battletimer.h"
#include <QDebug>
#include <QQmlComponent>

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    Controller controller(&engine);

    engine.rootContext()->setContextProperty("biCiFiModel", controller.get_BattleCityField());
    engine.rootContext()->setContextProperty("controllerMain",&controller);
    engine.rootContext()->setContextProperty("qmlBiCiFi",controller.get_QmlDataView());
    engine.rootContext()->setContextProperty("tankData",controller.get_QmlTankView());
    engine.rootContext()->setContextProperty("bullet_0_Data", controller.get_QmlBullet0List());
    engine.rootContext()->setContextProperty("bullet_1_Data", controller.get_QmlBullet1List());
    engine.rootContext()->setContextProperty("bullet_2_Data", controller.get_QmlBullet2List());
    engine.rootContext()->setContextProperty("bullet_3_Data", controller.get_QmlBullet3List());
    engine.rootContext()->setContextProperty("numberOfTank", controller.get_NumberOfTanks());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()){
        return -1;
    }
    return app.exec();
}
