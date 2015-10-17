/*
The MIT License (MIT)
Copyright © 2015 Jonas G. Drange <jonas@drange.net>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#include <algorithm>
#include <QtDebug>

#include "bulbmodel.h"

BulbModel::BulbModel(QObject *parent)
    : QAbstractListModel(parent) {
}

void BulbModel::addBulb(Lightbulb* bulb) {
    int at = rowCount();
    for (int i = 0; i < m_bulbs.size(); ++i) {
        if (bulb->label() < m_bulbs.at(i)->label()) {
            at = i;
            break;
        }
    }
    beginInsertRows(QModelIndex(), at, at);
    m_bulbs.insert(at, bulb);
    endInsertRows();
}

int BulbModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_bulbs.count();
}

QVariant BulbModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_bulbs.count())
        return QVariant();

    Lightbulb* bulb = m_bulbs[index.row()];
    if (role == Qt::DisplayRole) {
        return QVariant::fromValue(bulb);
    } else if (role == BulbRole) {
        return QVariant::fromValue(bulb);
    } else if (role == GroupRole) {
        return bulb->label();;
    }
    // if (role == TypeRole)
    //     return bulb.type();
    // else if (role == SizeRole)
    //     return bulb.size();
    return QVariant();
}

QHash<int, QByteArray> BulbModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[BulbRole] = "lifxbulb";
    roles[GroupRole] = "group";
    return roles;
}

BulbModel::~BulbModel() {
    qDeleteAll(m_bulbs);
}
