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
        QObject(parent)
{
    qRegisterMetaType<lifx::Header>("lifx::Header");

    BulbTracker *tracker = new BulbTracker();
    tracker->moveToThread(&trackerThread);
    connect(&trackerThread, &QThread::finished, tracker, &QObject::deleteLater);
    connect(this, &GlowControl::requestTrackerStart, tracker, &BulbTracker::start);
    connect(this, &GlowControl::requestTrackerStop, tracker, &BulbTracker::stop);
    connect(tracker, &BulbTracker::bulbReady, this, &GlowControl::handleBulb);
    trackerThread.start();
    requestTrackerStart();

    // Worker is our worker, it does talking to bulbs etc.
    BulbWorker *worker = new BulbWorker();
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &GlowControl::dispatchJob, worker, &BulbWorker::doJob);
    workerThread.start();
}

void GlowControl::setListeners(Lightbulb * bulb) {
    connect(bulb, &Lightbulb::requestSetProperty, this, &GlowControl::bulbRequestsSetProperty);
}

void GlowControl::bulbRequestsSetProperty(const QString &key, const QVariant &value) {
    Lightbulb* bulb = qobject_cast<Lightbulb *>(QObject::sender());
    if (key == QStringLiteral("Label")) {
        // TODO: implement
    } else if (key == QStringLiteral("Power")) {
        emit dispatchJob(QStringLiteral("power"), value.toBool(), bulb->header);
    } else if (key == QStringLiteral("Color")) {
        // TODO: implement
    } else if (key == QStringLiteral("Brightness")) {
        emit dispatchJob(QStringLiteral("brightness"), value.toInt(), bulb->header);
    }
}

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
        bulb->lifxSetsProperty("Label", QVariant(label));
        m_bulbs.append(bulb);
        std::stable_sort(m_bulbs.begin(), m_bulbs.end(), sort);
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

GlowControl::~GlowControl() {
    emit requestTrackerStop();
    trackerThread.quit();
    trackerThread.wait();
    workerThread.quit();
    workerThread.wait();
    qDeleteAll(m_bulbs);
}
