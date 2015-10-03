#ifndef GLOWCONTROL_H
#define GLOWCONTROL_H

#include <QObject>
#include <QThread>
#include <lib-lifx/lifx.h>

#include "lightbulb.h"

class Lightbulb;
class BulbDiscoverer;

class GlowControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QStringList bulbs READ bulbs NOTIFY bulbsChanged )
    QThread discovererThread;
public:
    explicit GlowControl(QObject *parent = 0);
    ~GlowControl();

Q_SIGNALS:
    void discoveryEnded();
    void bulbsChanged();

public slots:
    void handleBulb(const &Lightbulb);
    void handleResults(const &QList<Lightbulb>);

protected:
    QStringList bulbs() { return m_bulblist; }
    QStringList m_bulblist;

private:
    BulbList m_bulbs;

signals:
    void discover();
};

class BulbDiscoverer : public QObject
{
    Q_OBJECT
public:
    explicit BulbDiscoverer(QObject *parent = 0);
public slots:
    void discover();
signals:
    void bulbReady(const &Lightbulb);
    void resultReady(const &QList<Lightbulb>);
private:
    static uint64_t MacToNum(const uint8_t address[8]);
    template<typename T> void HandleCallback(std::function<void(Lightbulb& bulb, const T& msg)> func);
    lifx::LifxClient m_client;
    BulbList m_found_bulbs;
};

#endif // GLOWCONTROL_H

