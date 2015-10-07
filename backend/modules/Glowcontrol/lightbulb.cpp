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
    return m_label;
}

void Lightbulb::setMac(QString mac) {

}

void Lightbulb::setLabel(const QString &label) {
    m_label = label;
}

QString Lightbulb::group() {
    return m_group;
}

void Lightbulb::setGroup(const QString &group) {
    m_group = group;
}

bool Lightbulb::power() {
    return m_power;
}

void Lightbulb::setPower(bool power) {
    Q_EMIT powerChanged(power);
    m_power = power;
}

QVariant Lightbulb::color() {
    return m_color;
}

void Lightbulb::setColor(const QVariant &color) {
    m_color = color;
    Q_EMIT colorChanged(color);
}

int Lightbulb::brightness() {
    return m_brightness;
}

void Lightbulb::setBrightness(const int &brightness) {
    Q_EMIT brightnessChanged(brightness);
    m_brightness = brightness;
}

// std::array<uint8_t, 8> Lightbulb::mac() {
//     return m_mac_address;
// }

// void setMac(const std::array<uint8_t, 8> &mac) {
//     m_mac_address = mac;
// }

QString Lightbulb::version() {
    return QString();
}

void Lightbulb::setVersion(const uint32_t &vendor, const uint32_t &product, const uint32_t &version) {
    m_vendor = vendor;
    m_product = product;
    m_version = version;
}

void Lightbulb::propertyChanged(const QString &key, const QVariant &value) {

}
