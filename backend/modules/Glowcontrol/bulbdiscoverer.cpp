#include <QtDebug>

#include "bulbdiscoverer.h"

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

void BulbDiscoverer::discover() {

    qDebug() << "BulbDiscoverer" << __func__ << "discover";

    m_client.RegisterCallback<lifx::message::device::StateService>(
        [this](const lifx::Header& header, const lifx::message::device::StateService& msg) {
            if (msg.service == lifx::SERVICE_UDP) {
                QString a(QString::number(unsigned(MacToNum(header.target))));
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
                        // qWarning() << QString::fromStdString(msg.label) << msg.color.hue;
                        // double hue = double(msg.color.hue-0) / double(65535-0);
                        // double sat = double(msg.color.saturation-0) / double(65535-0);
                        // double bri = double(msg.color.brightness-0) / double(65535-0);
                        // double kel = 1.0;

                        // qWarning() << QString::fromStdString(msg.label) << t;

                        QMap<QString, QVariant> color;
                        color["hue"] = QVariant(msg.color.hue);
                        color["saturation"] = QVariant(msg.color.saturation);
                        color["brightness"] = QVariant(msg.color.brightness);
                        color["kelvin"] = QVariant(msg.color.kelvin);

                        QString a(QString::number(unsigned(MacToNum(header.target))));
                        m_found_bulbs.insert(a, QString::fromStdString(msg.label));
                        m_name_to_header[QString::fromStdString(msg.label)] = header;
                        emit bulbReady(QString::fromStdString(msg.label), msg.power > 0, color, header);
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
            // qDebug() << __func__ << num_identified << "/" << m_found_bulbs.size();

            if (num_identified == m_found_bulbs.size()) {
                //RunCommands(argc, argv);
                qDebug() << "Found all.";
                break;
            }
        }
        // qDebug() << count;
        if (count > 15000) {
            qDebug() << "time is money, giving up discovery";
            break;
        }
        count++;
        QThread::sleep(0.05);
    }
    qDebug() << "BulbDiscoverer" << __func__ << "Ending search";
    m_found_bulbs.clear();
    m_name_to_header.clear();
    emit done(true);
}

void BulbDiscoverer::requestState(const lifx::Header &header) {
    qDebug() << __func__;
    bool resp = false;
    lifx::Header targetHeader = header;
    // HandleCallback<lifx::message::light::State>(
    //     [this, &resp](const lifx::Header& header, const lifx::message::light::State& msg)
    //     {
    //         qDebug() << __func__ << "will talk back";
    //         QMap<QString, QVariant> color;
    //         color["hue"] = QVariant(msg.color.hue);
    //         color["saturation"] = QVariant(msg.color.saturation);
    //         color["brightness"] = QVariant(msg.color.brightness);
    //         color["kelvin"] = QVariant(msg.color.kelvin);

    //         emit bulbReady(QString::fromStdString(msg.label), msg.power > 0, color, header);
    //         resp = true;
    //     }
    // );

    m_client.RegisterCallback<lifx::message::light::State>(
        [this](const lifx::Header& header, const lifx::message::light::State& msg) {
            qDebug() << __func__ << "will talk back";
            QMap<QString, QVariant> color;
            color["hue"] = QVariant(msg.color.hue);
            color["saturation"] = QVariant(msg.color.saturation);
            color["brightness"] = QVariant(msg.color.brightness);
            color["kelvin"] = QVariant(msg.color.kelvin);
            emit bulbReady(QString::fromStdString(msg.label), msg.power > 0, color, header);
        }
    );

    m_client.Send<lifx::message::light::Get>(header.target);

    while (m_client.WaitingToSend()) {
        m_client.RunOnce();
    }

    // lifx::LifxClient::RunResult res;
    // int count = 0;
    // while (!resp) {
    //     res = m_client.RunOnce();
    //     qDebug() << __func__ << "loop" << count << (int)res;
    //     if (count > 100) {
    //         qDebug() << "giving up requestState, last run was" << (int)res;
    //         break;
    //     }
    //     QThread::msleep(100);
    //     count++;
    // }
}

BulbDiscoverer::~BulbDiscoverer() {}
