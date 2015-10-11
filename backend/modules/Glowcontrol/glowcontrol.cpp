#include <QtDebug>
#include <unistd.h>
#include <algorithm>

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
    BulbTracker *tracker = new BulbTracker();
    tracker->moveToThread(&trackerThread);
    // connect(&trackerThread, &QThread::finished, tracker, &QObject::deleteLater);
    connect(this, &GlowControl::requestTrackerStart, tracker, &BulbTracker::start);
    connect(this, &GlowControl::requestTrackerStop, tracker, &BulbTracker::stop);
    // connect(this, &GlowControl::stateRequest, tracker, &BulbTracker::requestState);
    connect(tracker, &BulbTracker::bulbReady, this, &GlowControl::handleBulb);
    //connect(tracker, &BulbTracker::done, this, &GlowControl::handleDiscoveryEnded);
    trackerThread.start();
    requestTrackerStart();

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

// bool GlowControl::discover() {
//     if (m_discovering) {
//         return false;
//     } else {
//         m_discovering = true;
//         emit discoveringChanged(m_discovering);
//         emit discoverRequest();
//         return true;
//     }
// }

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

// bool GlowControl::discovering() {
//     return m_discovering;
// }

// void GlowControl::handleDiscoveryEnded(const bool success) {
//     m_discovering = false;
//     emit discoveringChanged(m_discovering);
// }

// void GlowControl::handleBulbTalkback(const QString &label, const bool power, const QVariant &color, const lifx::Header &header) {
//     Lightbulb* bulb = m_name_to_bulb[label];
//     bulb->lifxSetsProperty("Label", QVariant(label));
//     bulb->lifxSetsProperty("Power", QVariant(power));
//     QMap<QString, QVariant> colorMap = color.toMap();
//     bulb->lifxSetsProperty("Color", color);
//     bulb->lifxSetsProperty("Brightness", colorMap["brightness"]);
// }

bool sort(Lightbulb* a, Lightbulb* b) {
    return a->label() < b->label();
}

void GlowControl::handleBulb(const QString &label, const bool power, const QVariant &color, const lifx::Header &header) {
    // m_bulblist << QString::fromStdString(bulb.label);

    // XXX: needs QPointer to guard
    // against dangling pointers if
    // a bulb qobj is ever deleted
    Lightbulb* bulb;
    if (!m_name_to_bulb.contains(label)) {
        bulb = new Lightbulb(this, header);
        setListeners(bulb);
        m_name_to_bulb.insert(label, bulb);

        m_bulbs.append(bulb);
        std::sort(m_bulbs.begin(), m_bulbs.end(), sort);
        Q_EMIT bulbsChanged();
        qDebug() << "GlowControl, saw new bulb:" << label;
    } else {
        bulb = m_name_to_bulb[label];
    }

    bulb->lifxSetsProperty("Label", QVariant(label));
    bulb->lifxSetsProperty("Power", QVariant(power));
    QMap<QString, QVariant> colorMap = color.toMap();
    bulb->lifxSetsProperty("Color", color);
    bulb->lifxSetsProperty("Brightness", colorMap["brightness"]);

}

void GlowControl::handleWorkerDone(const lifx::Header &header, const bool talkback) {
    if (talkback) {
        emit stateRequest(header);
    }
}

GlowControl::~GlowControl() {
    emit requestTrackerStop();
    trackerThread.quit();
    trackerThread.wait();
    workerThread.quit();
    workerThread.wait();
}
