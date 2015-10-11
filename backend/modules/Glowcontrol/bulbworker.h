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

The bulb worker does jobs. Currently it only knows a few.
*/

#ifndef BULBWORKER_H
#define BULBWORKER_H

#include <QObject>
#include <QQueue>
#include <lib-lifx/lifx.h>

class BulbWorker : public QObject
{
    Q_OBJECT

public:
    explicit BulbWorker(QObject *parent = 0);
    ~BulbWorker();

public slots:
    void doJob(const QString &type, const QVariant &arg, const lifx::Header &header);

signals:
    void done(const lifx::Header &header);
    void fail(const lifx::Header &header);

private:
    lifx::LifxClient m_client;
};

#endif // BULBWORKER_H
