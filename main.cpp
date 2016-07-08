#include <QApplication>
#include <QQmlApplicationEngine>
#include <QThread>

#include <QtWebEngine>
#include <QtQml>

#include "device.h"
#include "gasdata.h"
#include "daparameters.h"
#include "datasource.h"
#include "database.h"

#include <QDataStream>

qint16 adBuffer[1024];

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Database db(adBuffer);
    Device usb2089(adBuffer);
    QObject::connect(&usb2089, SIGNAL(acquired(const QDateTime&)), &db, SLOT(insert(const QDateTime&)));
    QObject::connect(&usb2089, SIGNAL(samplePeriodChanged(int)), &db, SLOT(onPeriodChanged(int)));

    QQmlApplicationEngine engine;
    QtWebEngine::initialize(); //WebEngineView init
    engine.addImportPath(QStringLiteral("qrc:/qml"));

    // C++/qml integration via Context Property
    engine.rootContext()->setContextProperty("device", &usb2089);
    engine.rootContext()->setContextProperty("gasdata", usb2089.m_gasData);
    engine.rootContext()->setContextProperty("adsource", usb2089.m_adSource);
    engine.rootContext()->setContextProperty("dasource", usb2089.m_daSource);
    engine.rootContext()->setContextProperty("daParameters", usb2089.m_daPara);
    engine.rootContext()->setContextProperty("database", &db);
    engine.load(QUrl("qrc:/qml/main.qml"));
    if (engine.rootObjects().isEmpty())
        qFatal("engine.rootObjects().isEmpty()");

    return app.exec();
}
