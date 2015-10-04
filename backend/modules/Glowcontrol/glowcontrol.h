#ifndef GLOWCONTROL_H
#define GLOWCONTROL_H

#include <QObject>
#include <QThread>
#include <QQmlListProperty>
#include <lib-lifx/lifx.h>

#include "lightbulb.h"

class BulbDiscoverer;

class GlowControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Lightbulb> bulbs READ getLightbulbList NOTIFY bulbsChanged)
    QThread discovererThread;
    QThread workerThread;
public:
    explicit GlowControl(QObject *parent = 0);
    ~GlowControl();

   QQmlListProperty<Lightbulb> getLightbulbList() {
      return QQmlListProperty<Lightbulb>(this, 0, &GlowControl::countLightbulbList, &GlowControl::atLightbulbList);
   }
   static int countLightbulbList(QQmlListProperty<Lightbulb> *property) {
      GlowControl *m = qobject_cast<GlowControl *>(property->object);
      return m->m_bulbs.size();
   }
   static Lightbulb *atLightbulbList(QQmlListProperty<Lightbulb> *property, int index) {
      GlowControl *m = qobject_cast<GlowControl *>(property->object);
      return m->m_bulbs[index];
   }

Q_SIGNALS:
    void discoveryEnded();
    void bulbsChanged();
    void bulbPowerRequest(const bool, const lifx::Header &header);
    void bulbRequest(const QString &type, const lifx::Header &header);

public slots:
    void handleBulb(const QString &label, const bool, const lifx::Header &header);
    void bulbPowerChanged(const bool);
    void handleDone(const lifx::Header &header);
    // void handleResults(const BulbMap &bulbMap);

private:
    // static void append_bulb(QQmlListProperty<Lightbulb> *list, Lightbulb *bulb);
    void setListeners(Lightbulb * bulb);
    QMap<QString, Lightbulb *> m_name_to_bulb;
    QList<Lightbulb *> m_bulbs;

signals:
    void discover();
};

class BulbDiscoverer : public QObject
{
    Q_OBJECT
public:
    explicit BulbDiscoverer(QObject *parent = 0);
    ~BulbDiscoverer();
public slots:
    void discover();
    void handlePowerRequest(const bool, const lifx::Header &header);
signals:
    void bulbReady(const QString &label, const bool, const lifx::Header &header);
    // void resultReady(const BulbMap &bulbMap);
private:
    static uint64_t MacToNum(const uint8_t address[8]);
    template<typename T> void HandleCallback(std::function<void(const lifx::Header &header, const T& msg)> func);
    lifx::LifxClient m_client;
    QMap<QString, QString> m_found_bulbs;
    QMap<QString, lifx::Header> m_name_to_header;
};

class BulbWorker : public QObject
{
    Q_OBJECT
public:
    explicit BulbWorker(QObject *parent = 0);
    ~BulbWorker();
public slots:
    void handleRequest(const QString &type, const lifx::Header &header);
signals:
    void done(const lifx::Header &header);
private:
    lifx::LifxClient m_client;
};

#endif // GLOWCONTROL_H

