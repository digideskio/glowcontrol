#include <QtDebug>
#include <QThread>

#include "bulbworker.h"

BulbWorker::BulbWorker(QObject *parent) :
    QObject(parent),
    m_client(1400)
{
    qDebug() << "Started worker" << 1400;
}

void BulbWorker::handleRequest(const QString &type, const QVariant &arg, const lifx::Header &header, const bool talkback) {
    qDebug() << "handle request" << type << arg << ", " << (talkback ? "and will talk back" : "will not talk back");

    std::array<uint8_t, 8> mac_address;
    for (auto i : {0, 1, 2, 3, 4, 5, 6, 7}) {
        mac_address[i] = header.target[i];
    }

    // TODO: Use enums.
    if (type == "power") {
        bool power = arg.toBool();
        qDebug() << "powering" << (power ? "up" : "down");
        if (power) {
            lifx::message::device::SetPower powerMsg{ 65535 };
            m_client.Send<lifx::message::device::SetPower>(powerMsg, mac_address.data());
        } else {
            m_client.Send<lifx::message::device::SetPower>(mac_address.data());
        }
    } else if (type == "brightness") {
        int brightness = arg.toInt();

        lifx::message::light::SetColor colorMsg {};
        colorMsg.color.brightness = brightness;
        m_client.Send<lifx::message::light::SetColor>(colorMsg, mac_address.data());
    }

    while (m_client.WaitingToSend()) {
        m_client.RunOnce();
    }

    // m_client.RegisterCallback<lifx::message::light::State>(
    //     [this](const lifx::Header& header, const lifx::message::light::State& msg) {
    //         QMap<QString, QVariant> color;
    //         color["hue"] = QVariant(msg.color.hue);
    //         color["saturation"] = QVariant(msg.color.saturation);
    //         color["brightness"] = QVariant(msg.color.brightness);
    //         color["kelvin"] = QVariant(msg.color.kelvin);
    //         emit bulbTalkback(QString::fromStdString(msg.label), msg.power > 0, color, header);
    //     }
    // );
    // m_client.Send<lifx::message::light::Get>(header.target);

    // while (m_client.WaitingToSend()) {
    //     m_client.RunOnce();
    // }
    emit done(header, talkback);
}

BulbWorker::~BulbWorker () {}
