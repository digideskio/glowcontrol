#include "lightbulb.h"

Lightbulb::Lightbulb(QObject *parent) :
    QObject(parent) {
}

Lightbulb::~Lightbulb() {}

QString Lightbulb::label() {
    return m_label;
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
    m_power = power;
}

QColor Lightbulb::color() {
    return QColor();
}

void Lightbulb::setColor(const QColor &color) {
    //
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

