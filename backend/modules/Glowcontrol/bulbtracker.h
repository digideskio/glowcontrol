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

The bulb tracker continuously requests state from known bulbs.
*/
#ifndef BULBTRACKER_H
#define BULBTRACKER_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include <lib-lifx/lifx.h>

class BulbTracker : public QObject
{
    Q_OBJECT

public:
    explicit BulbTracker(QObject *parent = 0, int interval = 1000);
    ~BulbTracker();

public slots:
    void start();
    void stop();

private slots:
    void update();

signals:
    void bulbReady(const QString &label, const bool power, const QVariant &color, const lifx::Header &header);
    void done(const bool success);
    void ping();

private:
    void registerCallbacks();
    void identify();
    static uint64_t MacToNum(const uint8_t address[8]);
    template<typename T> void HandleCallback(std::function<void(const lifx::Header &header, const T& msg)> func);
    lifx::LifxClient m_client;
    QMap<QString, QString> m_found_bulbs;
    QMap<QString, lifx::Header> m_name_to_header;
    QTimer *m_timer;
    int m_interval;
};

#endif // BULBTRACKER_H
