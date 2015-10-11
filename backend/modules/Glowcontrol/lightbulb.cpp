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

#include "lightbulb.h"

#define SUPER LifxObject

Lightbulb::Lightbulb(QObject *parent, lifx::Header header) :
    SUPER(parent),
    header(header)
{
}

Lightbulb::~Lightbulb() {}

QString Lightbulb::label() {
    return getProperty("Label").toString();
}

void Lightbulb::setLabel(const QString &label) {
    if (getProperty("Label").toString() != label) {
        setProperty("Label", QVariant(label));
    }
}

// XXX: non-functional
void Lightbulb::setMac(QString mac) {

}

// XXX: non-functional
QString Lightbulb::group() {
    return m_group;
}

// XXX: non-functional
void Lightbulb::setGroup(const QString &group) {
    m_group = group;
}

bool Lightbulb::power() {
    return getProperty("Power").toBool();
}

void Lightbulb::setPower(bool power) {
    if (getProperty("Power").toBool() != power) {
        setProperty("Power", QVariant(power));
    }
}

QVariant Lightbulb::color() {
    return getProperty("Color");
}

void Lightbulb::setColor(const QVariant &color) {
    if (getProperty("Color") != color) {
        setProperty("Color", color);
    }
}

int Lightbulb::brightness() {
    return getProperty("Brightness").toInt();
}

void Lightbulb::setBrightness(const int &brightness) {
    if (getProperty("Brightness").toInt() != brightness) {
        setProperty("Brightness", QVariant(brightness));
    }
}

// std::array<uint8_t, 8> Lightbulb::mac() {
//     return m_mac_address;
// }

// void setMac(const std::array<uint8_t, 8> &mac) {
//     m_mac_address = mac;
// }

// XXX: non-functional
QString Lightbulb::version() {
    return QString();
}

// XXX: non-functional
void Lightbulb::setVersion(const uint32_t &vendor, const uint32_t &product, const uint32_t &version) {
    m_vendor = vendor;
    m_product = product;
    m_version = version;
}

void Lightbulb::propertyChanged(const QString &key, const QVariant &value) {
    SUPER::propertyChanged(key, value);
    if (key == QStringLiteral("Label")) {
        Q_EMIT labelChanged(value.toString());
    } else if (key == QStringLiteral("Power")) {
        Q_EMIT powerChanged(value.toBool());
    } else if (key == QStringLiteral("Color")) {
        Q_EMIT colorChanged(value);
    } else if (key == QStringLiteral("Brightness")) {
        Q_EMIT brightnessChanged(value.toInt());
    }
}
