#include <QtQml>
#include <QtQml/QQmlContext>
#include "backend.h"
#include "lightbulb.h"
#include "glowcontrol.h"

void BackendPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Glowcontrol"));

    qRegisterMetaType <BulbList>("BulbList");
    qmlRegisterType<GlowControl>(uri, 1, 0, "GlowControl");
}

void BackendPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}

