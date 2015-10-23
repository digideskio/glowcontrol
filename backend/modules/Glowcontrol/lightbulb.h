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

Lightbulb represents a lifx lightbulb. Note that the bulb keeps state via the
LifxObject, and all user requests are simply relayed to the controller.
*/

#ifndef LIGHTBULB_H
#define LIGHTBULB_H

#include <QColor>
#include <QObject>

#include <lib-lifx/lifx.h>
#include "lifxobject.h"

// TODO: Proper signatures with variable names.
class Lightbulb : public LifxObject
{
    Q_OBJECT
    Q_PROPERTY( QString label READ label WRITE setLabel NOTIFY labelChanged )
    Q_PROPERTY( QString group READ group WRITE setGroup NOTIFY groupChanged )
    Q_PROPERTY( bool power READ power WRITE setPower NOTIFY powerChanged )
    Q_PROPERTY( QVariant color READ color WRITE setColor NOTIFY colorChanged )
    Q_PROPERTY( double brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged )
    // Q_PROPERTY( QString mac READ mac NOTIFY macChanged )
    Q_PROPERTY( QString version READ version NOTIFY versionChanged )
public:
    explicit Lightbulb(QObject *parent = 0, lifx::Header = {});
    ~Lightbulb();

    QString label();
    void setLabel(const QString &);

    QString group();
    void setGroup(const QString &);

    bool power();
    void setPower(bool);

    QVariant color();
    void setColor(const QVariant &color);

    double brightness();
    void setBrightness(const double &brightness);

    lifx::Header header;

    // void setMac(const std::array<uint8_t, 8> &);

    QString version();
    void setVersion(const uint32_t &, const uint32_t &, const uint32_t &);

public slots:
    void discover();

protected:
    void propertyChanged(const QString &key, const QVariant &value);

private:
    void setMac(QString mac);

    double normalizeBrightness(int brightness);
    int deNormalizeBrightness(double brightness);

    QString m_label;
    QString m_group;
    uint64_t m_group_updated_at;
    QString m_smac;

    uint32_t m_vendor;
    uint32_t m_product;
    uint32_t m_version;

    QString m_location;
    uint64_t m_location_updated_at;

signals:
    void labelChanged(const QString &label);
    void groupChanged(const QString &group);
    void powerChanged(const bool power);
    void colorChanged(const QVariant &color);
    void brightnessChanged(const double &brightness);
    void versionChanged(const QString &version);
// private:
//     static uint64_t MacToNum(const uint8_t address[8]);
//     template<typename T> void HandleCallback(std::function<void(Lightbulb& bulb, const T& msg)> func);
//     lifx::LifxClient m_client;
//     BulbList m_found_bulbs;
};

#endif
