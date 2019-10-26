#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSurfaceFormat>
#include "plineoffsetalgorithmview.h"


int main(int argc, char *argv[])
{
  qmlRegisterType<PlineOffsetAlgorithmView>("Polyline", 1, 0, "PlineOffsetAlgorithmView");
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  QSurfaceFormat format;
  format.setSamples(16);
  QSurfaceFormat::setDefaultFormat(format);
  QQmlApplicationEngine engine;

  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                   &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
  }, Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
