#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QQuickItem>
#include "qvlcplayer.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    //获取QMl的源对象
    auto qmlobj = engine.rootObjects().first();
    assert(qmlobj);

    // 显示区域窗口VLC播放器
    auto displayrect = qmlobj->findChild<QQuickItem*>("displaywnd");
    assert(displayrect);
    QVLCPlayer player;
    player.setRect(displayrect);

    // 将player添加到engine上下文,qml中就可以直接使用"player"
    engine.rootContext()->setContextProperty("player", &player);

    return app.exec();
}