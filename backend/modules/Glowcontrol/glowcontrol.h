#ifndef GLOWCONTROL_H
#define GLOWCONTROL_H

#include <QObject>
#include <QThread>
#include <QQmlListProperty>
#include <lib-lifx/lifx.h>

#include "bulbtracker.h"
#include "bulbworker.h"
#include "lightbulb.h"

class GlowControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Lightbulb> bulbs READ getLightbulbList NOTIFY bulbsChanged)
    // Q_PROPERTY(bool discovering READ discovering NOTIFY discoveringChanged)
    QThread trackerThread;
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
   bool discovering();

   Q_INVOKABLE bool discover();

Q_SIGNALS:
    // void discoveringChanged(const bool);
    void requestTrackerStart();
    void requestTrackerStop();
    // void discoveryDone();
    void stateRequest(const lifx::Header &header);
    void bulbsChanged();
    void bulbRequest(const QString &type, const QVariant &arg, const lifx::Header &header, const bool talkback = true);

public slots:
    void handleDiscoveryEnded(const bool success);
    void handleBulb(const QString &label, const bool power, const QVariant &color, const lifx::Header &header);
    void handleWorkerDone(const lifx::Header &header, const bool talkback);

    void bulbPowerChanged(const bool);
    void bulbColorChanged(const QVariant &color);
    void bulbBrightnessChanged(const int &brightness);
    void bulbRequestsSetProperty(const QString &key, const QVariant &value);

    // void handleResults(const BulbMap &bulbMap);

private:
    // static void append_bulb(QQmlListProperty<Lightbulb> *list, Lightbulb *bulb);
    void setListeners(Lightbulb * bulb);
    QMap<QString, Lightbulb *> m_name_to_bulb;
    QList<Lightbulb *> m_bulbs;
    bool m_discovering;
};

#endif // GLOWCONTROL_H
