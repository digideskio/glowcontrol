/*
* Copyright (C) 2015 Jonas G. Drange
* Copyright (C) 2014 Jolla Ltd.
*/

#include "lifxobject.h"

LifxObject::LifxObject(QObject *parent) :
    QObject(parent) { }

LifxObject::~LifxObject() { }

void LifxObject::setProperty(const QString &key, const QVariant &value) {
    emit requestSetProperty(key, value);
}

void LifxObject::updateProperty(const QString &key, const QVariant &value) {

    QVariant oldValue = getProperty(key);
    QVariant newValue;
    if (newValue.isValid()) {
        m_properties.insert(key, newValue);
    } else {
        m_properties.remove(key);
    }
    propertyChanged(key, newValue);
}

void LifxObject::onPropertyChanged(const QString &key, const QVariant &value) {
    updateProperty(key, value);
}

void LifxObject::propertyChanged(const QString &key, const QVariant &value) {}

QVariantMap LifxObject::getProperties() const {
    return m_properties;
}

QVariant LifxObject::getProperty(const QString &key) const {
    return m_properties[key];
}
