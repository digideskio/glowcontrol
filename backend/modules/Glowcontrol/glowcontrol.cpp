#include <QtDebug>
#include <unistd.h>
#include "glowcontrol.h"

class Lightbulb;
// qRegisterMetaType<Lightbulb>("Lightbulb");

std::unordered_map<std::string, lifx::HSBK> colors =
{
  { "red", { 62978, 65535, 65535, 3500 }},
  { "orange", { 5525, 65535, 65535, 3500 }},
  { "yellow", { 7615, 65535, 65535, 3500 } },
  { "green", { 16173, 65535, 65535, 3500 } },
  { "cyan", { 29814, 65535, 65535, 3500 } },
  { "blue", { 43634, 65535, 65535, 3500 } },
  { "purple", { 50486, 65535, 65535, 3500 } },
  { "pink", { 58275, 65535, 47142, 3500 } },
  { "white", { 58275, 0, 65535, 5500 } },
  { "cold_white", { 58275, 0, 65535, 9000 } },
  { "warm_white", { 58275, 0, 65535, 3200 } },
  { "gold", { 58275, 0, 65535, 2500 } },
};

GlowControl::GlowControl(QObject *parent) :
        QObject(parent),
        m_discovering(false)
{
    qRegisterMetaType<lifx::Header>("lifx::Header");

    // Discoverer will do discovery at startup and when refresh is clicked.
    BulbDiscoverer *discoverer = new BulbDiscoverer();
    discoverer->moveToThread(&discovererThread);
    // connect(&discovererThread, &QThread::finished, discoverer, &QObject::deleteLater);
    connect(this, &GlowControl::discoverRequest, discoverer, &BulbDiscoverer::discover);
    connect(this, &GlowControl::stateRequest, discoverer, &BulbDiscoverer::requestState);
    connect(discoverer, &BulbDiscoverer::bulbReady, this, &GlowControl::handleBulb);
    connect(discoverer, &BulbDiscoverer::done, this, &GlowControl::handleDiscoveryEnded);
    discovererThread.start();
    discover();

    // Worker is our worker, it does talking to bulbs etc.
    BulbWorker *worker = new BulbWorker();
    worker->moveToThread(&workerThread);
    connect(this, &GlowControl::bulbRequest, worker, &BulbWorker::handleRequest);
    connect(worker, &BulbWorker::done, this, &GlowControl::handleWorkerDone);
    workerThread.start();
}

void GlowControl::setListeners(Lightbulb * bulb) {
    connect(bulb, &Lightbulb::requestSetProperty, this, &GlowControl::bulbRequestsSetProperty);
}

bool GlowControl::discover() {
    qWarning() << "GlowControl" << __func__ << "discover";
    if (m_discovering) {
        return false;
    } else {
        m_discovering = true;
        emit discoveringChanged(m_discovering);
        emit discoverRequest();
        return true;
    }
}

void GlowControl::bulbRequestsSetProperty(const QString &key, const QVariant &value) {
    Lightbulb* bulb = qobject_cast<Lightbulb *>(QObject::sender());
    if (key == QStringLiteral("Label")) {
        // TODO: implement
    } else if (key == QStringLiteral("Power")) {
        emit bulbRequest(QStringLiteral("power"), value.toBool(), bulb->header);
    } else if (key == QStringLiteral("Color")) {
        // TODO: implement
    } else if (key == QStringLiteral("Brightness")) {
        emit bulbRequest(QString("brightness"), value.toInt(), bulb->header, false);
    }
}

bool GlowControl::discovering() {
    return m_discovering;
}

void GlowControl::handleDiscoveryEnded(const bool success) {
    qWarning() << "handleDiscoveryEnded";
    m_discovering = false;
    emit discoveringChanged(m_discovering);
}

// void GlowControl::handleBulbTalkback(const QString &label, const bool power, const QVariant &color, const lifx::Header &header) {
//     qDebug() << __func__ << label << power << color;
//     Lightbulb* bulb = m_name_to_bulb[label];
//     bulb->lifxSetsProperty("Label", QVariant(label));
//     bulb->lifxSetsProperty("Power", QVariant(power));
//     QMap<QString, QVariant> colorMap = color.toMap();
//     bulb->lifxSetsProperty("Color", color);
//     bulb->lifxSetsProperty("Brightness", colorMap["brightness"]);
// }

void GlowControl::handleBulb(const QString &label, const bool power, const QVariant &color, const lifx::Header &header) {
    // qDebug() << __PRETTY_FUNCTION__ << "handleBulb" << label;
    // m_bulblist << QString::fromStdString(bulb.label);

    // XXX: needs QPointer to guard
    // against dangling pointers if
    // a bulb qobj is ever deleted
    Lightbulb* bulb;
    if (!m_name_to_bulb.contains(label)) {
        bulb = new Lightbulb(this, header);
        qWarning() << "new bulb" << label;
        setListeners(bulb);
        // qWarning() << "created bulb" << bulb->label();
        m_name_to_bulb.insert(label, bulb);
        m_bulbs.append(bulb);
        Q_EMIT bulbsChanged();
    } else {
        bulb = m_name_to_bulb[label];
        qWarning() << "existing bulb" << label;
    }

    bulb->lifxSetsProperty("Label", QVariant(label));
    bulb->lifxSetsProperty("Power", QVariant(power));
    QMap<QString, QVariant> colorMap = color.toMap();
    bulb->lifxSetsProperty("Color", color);
    bulb->lifxSetsProperty("Brightness", colorMap["brightness"]);
    qWarning() << label << (power ? "now on" : "now off") << ", brightness:" << colorMap["brightness"];

}

void GlowControl::handleWorkerDone(const lifx::Header &header, const bool talkback) {
    qDebug() << __func__ << "will" << (talkback ? "" : "not") << "talkback";
    if (talkback) {
        emit stateRequest(header);
    }
}

GlowControl::~GlowControl() {
    discovererThread.quit();
    discovererThread.wait();
    workerThread.quit();
    workerThread.wait();
}
