/*
The MIT License (MIT)
Copyright © 2014 Jolla Ltd.
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

The lifx object is a property cache that lifx lightbulbs can use to
know the most recent update from any one bulb.
*/

#ifndef LIFXOBJECT_H
#define LIFXOBJECT_H

#include <QVariant>

class LifxObject : public QObject
{
    Q_OBJECT

public:
    void lifxSetsProperty(const QString &key, const QVariant &value);

Q_SIGNALS:
    void requestSetProperty(const QString &key, const QVariant &value);

protected:
    LifxObject(QObject *parent = 0);
    ~LifxObject();

    virtual void propertyChanged(const QString &key, const QVariant &value);
    void setProperty(const QString &key, const QVariant &value);
    // TODO: Implement sync mutator.
    // bool setPropertySync(const QString &key, const QVariant &value);
    void updateProperty(const QString &key, const QVariant &value);

    QVariantMap getProperties() const;
    QVariant getProperty(const QString &key) const;


private:
    QVariantMap m_properties;
    static bool safeVariantEq(const QVariant &v1, const QVariant &v2);

};

#endif // LIFXOBJECT_H
