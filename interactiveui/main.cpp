#include "demofuncs.h"
#include "plinecombinealgorithmview.h"
#include "plineoffsetalgorithmview.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSurfaceFormat>

int main(int argc, char *argv[]) {
  qmlRegisterSingletonType<DemoFuncs>("DemoFuncs", 1, 0, "DemoFuncs",
                                      [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
                                        Q_UNUSED(engine)
                                        Q_UNUSED(scriptEngine)

                                        DemoFuncs *demoFuncs = new DemoFuncs();
                                        return demoFuncs;
                                      });

  qmlRegisterType<PlineOffsetAlgorithmView>("Polyline", 1, 0, "PlineOffsetAlgorithmView");
  qmlRegisterType<PlineCombineAlgorithmView>("Polyline", 1, 0, "PlineCombineAlgorithmView");

  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);
  QFont defaultFont(app.font().family(), 12);
  app.setFont(defaultFont);

  QSurfaceFormat format;
  format.setSamples(16);
  QSurfaceFormat::setDefaultFormat(format);

  QQmlApplicationEngine engine;

  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
                   [url](QObject *obj, const QUrl &objUrl) {
                     if (!obj && url == objUrl)
                       QCoreApplication::exit(-1);
                   },
                   Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
