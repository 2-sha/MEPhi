#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTextCodec>

#include "Backend.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8")); //изменения
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<Backend>("Backend", 1, 0, "Backend", [](QQmlEngine* engine, QJSEngine* scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        Backend* backend = new Backend();
        return backend;
    });

    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
