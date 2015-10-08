/*
* Copyright (C) 2015 Jonas G. Drange
* Copyright (C) 2014 Jolla Ltd.
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
