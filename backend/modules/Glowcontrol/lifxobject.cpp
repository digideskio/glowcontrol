/*
* Copyright (C) 2015 Jonas G. Drange
* Copyright (C) 2014 Jolla Ltd.
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
