#ifndef BULBTRACKER_H
#define BULBTRACKER_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include <lib-lifx/lifx.h>

class BulbTracker : public QObject
{
    Q_OBJECT

public:
    explicit BulbTracker(QObject *parent = 0, int interval = 1000);
    ~BulbTracker();

public slots:
    void start();
    void stop();

private slots:
    void update();

signals:
    void bulbReady(const QString &label, const bool power, const QVariant &color, const lifx::Header &header);
    void done(const bool success);
    void ping();

private:
    void registerCallbacks();
    void identify();
    static uint64_t MacToNum(const uint8_t address[8]);
    template<typename T> void HandleCallback(std::function<void(const lifx::Header &header, const T& msg)> func);
    lifx::LifxClient m_client;
    QMap<QString, QString> m_found_bulbs;
    QMap<QString, lifx::Header> m_name_to_header;
    QTimer *m_timer;
    int m_interval;
};

#endif // BULBTRACKER_H
