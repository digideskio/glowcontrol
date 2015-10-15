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

#ifndef LIFXHELPER_H
#define LIFXHELPER_H

#include <QtAlgorithms>
#include <QtDebug>

#include <lib-lifx/lifx_messages.h>

class LifxHelper : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(LifxHelper)
    Q_PROPERTY(QStringList colors READ colorList CONSTANT)
    LifxHelper() {


        QMap<QString, int> m_hues{{
            { "red", 62978 },
            { "orange", 5525 },
            { "yellow", 7615 },
            { "green", 16173 },
            { "cyan", 29814 },
            { "blue", 43634 },
            { "purple", 50486 },
            { "pink", 58275 }
        }};

        for(QString e : m_hues.keys()) {
            m_colorList.append(e);
        }

        std::sort(m_colorList.begin(), m_colorList.end());
    }

public:
    QStringList colorList() {
        return m_colorList;
    }

    Q_INVOKABLE lifx::HSBK colorToHSKB(const QString &color) {
        return toHSKB(color, 65535);
    }

    Q_INVOKABLE lifx::HSBK colorToHSKB(const QString &color, const int &saturation) {
        return toHSKB(color, saturation);
    }

    Q_INVOKABLE lifx::HSBK colorToHSKB(const QString &color, const double &saturation) {
        return toHSKB(color, (int)(65535 * saturation));
    }

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine);
        Q_UNUSED(scriptEngine);

        return new LifxHelper;
    }

private:

    lifx::HSBK toHSKB(const QString &color, const int &saturation) {
        lifx::HSBK c;
        c.hue = m_hues[color];
        c.saturation = saturation;
        return c;
    };

    QMap<QString, int> m_hues;

    QStringList m_colorList;
};

#endif // LIFXHELPER_H
