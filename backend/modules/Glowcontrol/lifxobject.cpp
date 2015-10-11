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
*/

#include <QtDebug>
#include "lifxobject.h"

LifxObject::LifxObject(QObject *parent) :
    QObject(parent) { }

LifxObject::~LifxObject() { }

void LifxObject::setProperty(const QString &key, const QVariant &value) {
    emit requestSetProperty(key, value);
}

void LifxObject::updateProperty(const QString &key, const QVariant &value) {
    QVariant oldValue = getProperty(key);
    if (!LifxObject::safeVariantEq(value, oldValue)) {
        if (value.isValid()) {
            m_properties.insert(key, value);
        } else {
            m_properties.remove(key);
        }
        propertyChanged(key, value);
    }
}

void LifxObject::propertyChanged(const QString &key, const QVariant &value) {}

QVariantMap LifxObject::getProperties() const {
    return m_properties;
}

QVariant LifxObject::getProperty(const QString &key) const {
    return m_properties[key];
}

void LifxObject::lifxSetsProperty(const QString &key, const QVariant &value) {
    updateProperty(key, value);
}

bool LifxObject::safeVariantEq(const QVariant &v1, const QVariant &v2)
{
    if (!v1.isValid() && !v2.isValid())
        return true;
    if (!v1.isValid() && v2 == QVariant(v2.type()))
        return true;
    if (!v2.isValid() && v1 == QVariant(v1.type()))
        return true;
    return v1 == v2;
}
