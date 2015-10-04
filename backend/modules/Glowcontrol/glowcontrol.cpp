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
        QObject(parent)
{
    qRegisterMetaType<lifx::Header>("lifx::Header");

    BulbDiscoverer *discoverer = new BulbDiscoverer();
    discoverer->moveToThread(&discovererThread);
    connect(&discovererThread, &QThread::finished, discoverer, &QObject::deleteLater);
    connect(this, &GlowControl::discover, discoverer, &BulbDiscoverer::discover);
    connect(discoverer, &BulbDiscoverer::bulbReady, this, &GlowControl::handleBulb);
    connect(this, &GlowControl::bulbPowerRequest, discoverer, &BulbDiscoverer::handlePowerRequest);
    // connect(discoverer, &BulbDiscoverer::resultReady, this, &GlowControl::handleResults);
    discovererThread.start();
    emit discover();

    BulbWorker *worker = new BulbWorker();
    worker->moveToThread(&workerThread);
    connect(this, &GlowControl::bulbRequest, worker, &BulbWorker::handleRequest);
    connect(worker, &BulbWorker::done, this, &GlowControl::handleDone);
    // connect(discoverer, &BulbDiscoverer::resultReady, this, &GlowControl::handleResults);
    workerThread.start();
}

void GlowControl::setListeners(Lightbulb * bulb) {
    connect(bulb, &Lightbulb::powerChanged, this, &GlowControl::bulbPowerChanged);
}

void GlowControl::bulbPowerChanged(const bool power) {
    if (power) {
        emit bulbRequest(QString("power-on"), qobject_cast<Lightbulb *>(QObject::sender())->header);
    } else {
        emit bulbRequest(QString("power-off"), qobject_cast<Lightbulb *>(QObject::sender())->header);
    }
    qWarning() << "saw bulb power changed" << qobject_cast<Lightbulb *>(QObject::sender())->label() << power;
    //emit bulbPowerRequest(power, qobject_cast<Lightbulb *>(QObject::sender())->header);
}

void GlowControl::handleDone(const lifx::Header &header) {
    qDebug() << "handleDone";
}

// void GlowControl::handleResults(const BulbMap &bulbMap) {
//     qDebug() << __PRETTY_FUNCTION__ << "handleResults" << bulbMap.size();
//     // for (auto it = list.begin(); it != list.end(); ++it) {
//     //     m_bulblist << QString::fromStdString(it->second.label);
//     // }
//     // Q_EMIT bulbsChanged();
//     // Q_EMIT discoveryEnded();
//     // for(auto b : bulbMap.keys()) {
//     //   // fout << e << "," << extensions.value(e) << '\n';
//     //     m_bulbs.append(bulbMap.value(b));
//     // }
//     // Q_EMIT bulbsChanged();
// }

void GlowControl::handleBulb(const QString &label, const bool power, const lifx::Header &header) {
    qDebug() << __PRETTY_FUNCTION__ << "handleBulb" << label;
    // m_bulblist << QString::fromStdString(bulb.label);

    if (!m_name_to_bulb.contains(label)) {
        Lightbulb* bulb = new Lightbulb(this, header);
        bulb->setLabel(label);
        bulb->setPower(power);
        setListeners(bulb);
        qWarning() << "created bulb" << bulb->label();
        m_name_to_bulb.insert(label, bulb);
        m_bulbs.append(bulb);
        Q_EMIT bulbsChanged();
    }
}

// void GlowControl::append_bulb(QQmlListProperty<Lightbulb> *list, Lightbulb *bulb) {
//     GlowControl *gControl = qobject_cast<GlowControl *>(list->object);
//     if (bulb)
//         gControl->m_bulbs.append(bulb);
// }


GlowControl::~GlowControl() {
    discovererThread.quit();
    discovererThread.wait();
    workerThread.quit();
    workerThread.wait();
}

BulbDiscoverer::BulbDiscoverer(QObject *parent) :
    QObject(parent),
    m_client(1300)
{
    qDebug() << "Started discoverer" << 1300;
}

template<typename T>
void BulbDiscoverer::HandleCallback(
        std::function<void(const lifx::Header& header, const T& msg)> func) {
    m_client.RegisterCallback<T>(
        [func = std::move(func), this]
        (const lifx::Header& header, const T& msg) {
            QString m(QString::number(unsigned(MacToNum(header.target))));
            if (m_found_bulbs.contains(m)) {
                func(header, msg);
            }
        }
    );
}

uint64_t BulbDiscoverer::MacToNum(const uint8_t address[8]) {
    uint64_t num =
        static_cast<uint64_t>(address[0]) << 56 |
        static_cast<uint64_t>(address[1]) << 48 |
        static_cast<uint64_t>(address[2]) << 40 |
        static_cast<uint64_t>(address[3]) << 32 |
        address[4] << 24 |
        address[5] << 16 |
        address[6] << 8 |
        address[7] << 0;
    return std::move(num);
}

void BulbDiscoverer::handlePowerRequest(const bool, const lifx::Header &header) {
    qDebug() << "handlePowerRequest";
}

void BulbDiscoverer::discover() {

    qDebug() << __func__ << "discover";

    m_client.RegisterCallback<lifx::message::device::StateService>(
        [this](const lifx::Header& header, const lifx::message::device::StateService& msg) {
            qWarning() << "a";
            if (msg.service == lifx::SERVICE_UDP) {
                QString a(QString::number(unsigned(MacToNum(header.target))));
                qWarning() << "insert" << MacToNum(header.target);
                m_found_bulbs.insert(a, QString());
                // BulbDiscoverer::MacToNum(header.target)
                // HandleCallback<lifx::message::device::StateLocation>(
                //     [](Lightbulb* bulb, const lifx::message::device::StateLocation& msg)
                //     {
                //         bulb.location = { msg.label, msg.updated_at };
                //     }
                // );

                // HandleCallback<lifx::message::device::StateVersion>(
                //     [this](Lightbulb& bulb, const lifx::message::device::StateVersion& msg)
                //     {
                //         bulb->setVersion(msg.vendor, msg.product, msg.version);

                //         // m_client.Send<lifx::message::device::GetLocation>(bulb.mac.data());
                //     }
                // );

                // HandleCallback<lifx::message::device::StateGroup>(
                //     [this](Lightbulb& bulb, const lifx::message::device::StateGroup& msg)
                //     {
                //         bulb.group = { msg.label, msg.updated_at };

                //         m_client.Send<lifx::message::device::GetVersion>(bulb.mac.data());
                //     }
                // );

                HandleCallback<lifx::message::light::State>(
                    [this](const lifx::Header& header, const lifx::message::light::State& msg)
                    {
                        // bulb->setPower(msg.power > 0);
                        // bulb->setLabel(QString::fromStdString(msg.label));
                        // qWarning() << "bulb label now" << bulb->label();
                        // bulb.color = msg.color;
                        QString a(QString::number(unsigned(MacToNum(header.target))));
                        m_found_bulbs.insert(a, QString::fromStdString(msg.label));
                        m_name_to_header[QString::fromStdString(msg.label)] = header;
                        emit bulbReady(QString::fromStdString(msg.label), msg.power > 0, header);
                        // qDebug() << __func__ << "bulbReady" << QString::fromStdString(bulb.label);

                        // m_client.Send<lifx::message::device::GetGroup>(bulb.mac.data());
                    }
                );
                m_client.Send<lifx::message::light::Get>(header.target);
            }
        }
    );

    m_client.Broadcast<lifx::message::device::GetService>({});
    unsigned int num_identified = 0;

    int count = 0;
    for (;;) {
        m_client.RunOnce();

        if (!m_found_bulbs.empty()) {
            num_identified = 0;
            // std::for_each(
            //     m_found_bulbs.cbegin(), m_found_bulbs.cend(),
            //     [&num_identified](const std::pair<uint64_t, *Lightbulb>& entry) {
            //         // if (!entry.second.location.label.empty() && entry.second.location.updated_at != 0) {
            //         if (!entry.second->location.label.empty()) {
            //             ++num_identified;
            //         }
            //     }
            // );
            for(auto b : m_found_bulbs.keys()) {
              // fout << e << "," << extensions.value(e) << '\n';
                if (!m_found_bulbs.value(b).isEmpty()) {
                    num_identified++;
                }
            }
            qDebug() << __func__ << num_identified << "/" << m_found_bulbs.size();

            if (num_identified == m_found_bulbs.size()) {
                //RunCommands(argc, argv);
                qDebug() << "RunCommands";
                break;
            }
        }
        qDebug() << count;
        if (count > 100000) {
            qDebug() << "time is money, giving up discovery";
            break;
        }
        count++;
        QThread::sleep(0.05);
    }
    qDebug() << __func__ << "Ending search";
    // emit resultReady(m_found_bulbs);
}

BulbDiscoverer::~BulbDiscoverer() {}

BulbWorker::BulbWorker(QObject *parent) :
    QObject(parent),
    m_client(1400)
{
    qDebug() << "Started worker" << 1400;
}

void BulbWorker::handleRequest(const QString &type, const lifx::Header &header) {
    qWarning() << "handle request" << type;
    std::array<uint8_t, 8> mac_address;
    for (auto i : {0, 1, 2, 3, 4, 5, 6, 7}) {
        mac_address[i] = header.target[i];
    }

    if (type == "power-off") {
        qWarning() << "powering off";
        m_client.Send<lifx::message::device::SetPower>(mac_address.data());
    } else if (type == "power-on") {
        qWarning() << "powering on";
        lifx::message::device::SetPower powerMsg{ 65535 };
        m_client.Send<lifx::message::device::SetPower>(powerMsg, mac_address.data());
    }

    while (m_client.WaitingToSend())
    {
        m_client.RunOnce();
    }

    emit done(header);
}

BulbWorker::~BulbWorker () {}
