#ifndef BULBS_H
#define BULBS_H

#include <QAbstractItemModel>

#include "lightbulb.h"

class Bulbs : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    explicit Bulbs(QObject *parent = 0);

    int rowCount(const QModelIndex & = QModelIndex()) const override { return m_data.count(); }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE Lightbulb* get(int index) const { return m_data.at(index); }

    bool insertRow(int row, const QModelIndex & parent = QModelIndex());

signals:
    void countChanged(int c);

private:
    QList<Lightbulb *> m_data;
};

#endif
