/*
The MIT License (MIT)
Copyright © 2015 Jonas G. Drange <jonas@drange.net>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <QtDebug>
#include <QThread>

#include "bulbworker.h"

BulbWorker::BulbWorker(QObject *parent) :
    QObject(parent),
    m_client(1400)
{
}

void BulbWorker::doJob(const QString &type, const QVariant &arg, const lifx::Header &header) {

    // XXX: Do not assume there's a mac here. If there's none, make it a broadcast.
    std::array<uint8_t, 8> mac_address;
    for (auto i : {0, 1, 2, 3, 4, 5, 6, 7}) {
        mac_address[i] = header.target[i];
    }

    // TODO: Use enums.
    if (type == "power") {
        bool power = arg.toBool();
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

    emit done(header);
}

BulbWorker::~BulbWorker () {}
