#ifndef BULBDISCOVERER_H
#define BULBDISCOVERER_H

#include <QObject>
#include <QThread>

#include <lib-lifx/lifx.h>

class BulbDiscoverer : public QObject
{
    Q_OBJECT
public:
    explicit BulbDiscoverer(QObject *parent = 0);
    ~BulbDiscoverer();
public slots:
    void discover();
    void requestState(const lifx::Header &header);
signals:
    void bulbReady(const QString &label, const bool power, const QVariant &color, const lifx::Header &header);
    void done(const bool success);
private:
    static uint64_t MacToNum(const uint8_t address[8]);
    template<typename T> void HandleCallback(std::function<void(const lifx::Header &header, const T& msg)> func);
    lifx::LifxClient m_client;
    QMap<QString, QString> m_found_bulbs;
    QMap<QString, lifx::Header> m_name_to_header;
};

#endif // BULBDISCOVERER_H
