#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "QYBlueTooth/qybluetoothclient.h"
#include "QYBlueTooth/qybluetoothserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // 服务器
    QYBlueToothServer server;
    engine.rootContext()->setContextProperty("server", &server);
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    // 客户端
//    QYBlueToothClient client;
//    engine.rootContext()->setContextProperty("client", &client);
//    const QUrl url(QStringLiteral("qrc:/Client.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
