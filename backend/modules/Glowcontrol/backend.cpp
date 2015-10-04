#include <QtQml>
#include <QtQml/QQmlContext>
#include "backend.h"
#include "lightbulb.h"
#include "glowcontrol.h"

void BackendPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Glowcontrol"));

    qmlRegisterType<GlowControl>(uri, 1, 0, "GlowControl");
    // qmlRegisterType<Lightbulb>(uri, 1, 0, "Lightbulb");
    qmlRegisterUncreatableType<Lightbulb,1>(uri, 1, 0, "Lightbulb","error message");
}

void BackendPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}

