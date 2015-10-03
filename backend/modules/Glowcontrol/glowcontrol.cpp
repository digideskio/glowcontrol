#include <QtDebug>
#include "glowcontrol.h"

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
        QObject(parent) {
    BulbDiscoverer *discoverer = new BulbDiscoverer();
    discoverer->moveToThread(&discovererThread);
    connect(&discovererThread, &QThread::finished, discoverer, &QObject::deleteLater);
    connect(this, &GlowControl::discover, discoverer, &BulbDiscoverer::discover);
    connect(discoverer, &BulbDiscoverer::resultReady, this, &GlowControl::handleResults);
    connect(discoverer, &BulbDiscoverer::bulbReady, this, &GlowControl::handleBulb);
    // discovererThread.start();
    // emit discover();
}


void GlowControl::handleResults(const BulbList &list) {
    qDebug() << __PRETTY_FUNCTION__ << "handleResults" << list.size();
    for (auto it = list.begin(); it != list.end(); ++it) {
        m_bulblist << QString::fromStdString(it->second.label);
    }
    Q_EMIT bulbsChanged();
    Q_EMIT discoveryEnded();
}

void GlowControl::handleBulb(const Lightbulb &bulb) {
    qDebug() << __PRETTY_FUNCTION__ << "handleBulb" << QString::fromStdString(bulb.label);
    m_bulblist << QString::fromStdString(bulb.label);
    Q_EMIT bulbsChanged();
}

GlowControl::~GlowControl() {
    discovererThread.quit();
    discovererThread.wait();
}

BulbDiscoverer::BulbDiscoverer(QObject *parent) :
    QObject(parent),
    m_client()
{
}

template<typename T>
void BulbDiscoverer::HandleCallback(
        std::function<void(Lightbulb& bulb, const T& msg)> func) {
    m_client.RegisterCallback<T>(
        [func = std::move(func), this]
        (const lifx::Header& header, const T& msg) {
            auto num = MacToNum(header.target);
            auto bulb = m_found_bulbs.find(num);
            if (bulb != m_found_bulbs.end()) {
                func(bulb->second, msg);
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

void BulbDiscoverer::discover() {

    qDebug() << __func__ << "discover";

    m_client.RegisterCallback<lifx::message::device::StateService>(
        [this](const lifx::Header& header, const lifx::message::device::StateService& msg)
        {
            if (msg.service == lifx::SERVICE_UDP) {
                Lightbulb bulb();

                for (auto i : {0, 1, 2, 3, 4, 5, 6, 7}){
                        bulb.mac[i] = header.target[i];
                }

                m_found_bulbs[BulbDiscoverer::MacToNum(header.target)] = &bulb;

                // HandleCallback<lifx::message::device::StateLocation>(
                //     [](Lightbulb& bulb, const lifx::message::device::StateLocation& msg)
                //     {
                //         bulb.location = { msg.label, msg.updated_at };
                //     }
                // );

                HandleCallback<lifx::message::device::StateVersion>(
                    [this](Lightbulb& bulb, const lifx::message::device::StateVersion& msg)
                    {
                        bulb.setVersion(msg.vendor, msg.product, msg.version);

                        m_client.Send<lifx::message::device::GetLocation>(bulb.mac.data());
                    }
                );

                HandleCallback<lifx::message::device::StateGroup>(
                    [this](Lightbulb& bulb, const lifx::message::device::StateGroup& msg)
                    {
                        bulb.group = { msg.label, msg.updated_at };

                        m_client.Send<lifx::message::device::GetVersion>(bulb.mac.data());
                    }
                );

                HandleCallback<lifx::message::light::State>(
                    [this](Lightbulb& bulb, const lifx::message::light::State& msg)
                    {
                        bulb.setPower(msg.power > 0);
                        bulb.setLabel(QString::fromStdString(msg.label));
                        // bulb.color = msg.color;

                        emit bulbReady(bulb);
                        qDebug() << __func__ << "bulbReady" << QString::fromStdString(bulb.label);

                        m_client.Send<lifx::message::device::GetGroup>(bulb.mac.data());
                    });

                m_client.Send<lifx::message::light::Get>(bulb.mac.data());
        }
    });

    m_client.Broadcast<lifx::message::device::GetService>({});
    unsigned int num_identified = 0;
    qDebug() << __func__ << "Starting search. Saw " << m_found_bulbs.size() << "bulbs.";
    for (;;) {
        m_client.RunOnce();

        if (!m_found_bulbs.empty()) {
            num_identified = 0;
            std::for_each(
                m_found_bulbs.cbegin(), m_found_bulbs.cend(),
                [&num_identified](const std::pair<uint64_t, *Lightbulb>& entry) {
                    // if (!entry.second.location.label.empty() && entry.second.location.updated_at != 0) {
                    if (!entry.second->location.label.empty()) {
                        ++num_identified;
                    }
                }
            );
            // qDebug() << __func__ << num_identified << "/" << m_found_bulbs.size();

            if (num_identified == m_found_bulbs.size()) {
                //RunCommands(argc, argv);
                qDebug() << "RunCommands";
                break;
            }
        }
    }
    qDebug() << __func__ << "Ending search";
    // emit resultReady(m_found_bulbs);
}

BulbDiscoverer::~BulbDiscoverer {

}
