#ifndef BULBWORKER_H
#define BULBWORKER_H

#include <QObject>
#include <QQueue>
#include <lib-lifx/lifx.h>

class BulbWorker : public QObject
{
    Q_OBJECT

public:
    explicit BulbWorker(QObject *parent = 0);
    ~BulbWorker();

public slots:
    void handleRequest(const QString &type, const QVariant &arg, const lifx::Header &header, const bool talkback);

signals:
    void done(const lifx::Header &header, const bool talkback);
    void fail(const lifx::Header &header);

private:
    lifx::LifxClient m_client;
};

#endif // BULBWORKER_H
