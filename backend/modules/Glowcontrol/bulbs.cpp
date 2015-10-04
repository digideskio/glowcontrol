#include "bulbs.h"

Bulbs::Bulbs(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariant Bulbs::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
        return QVariant();

    Lightbulb* val = m_data.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return val->label();
        break;
    // case Qt::DecorationRole:
    //     return QColor(val & 0x1 ? Qt::red : Qt::green);
    //     break;
    // case Qt::EditRole:
    //     return QString::number(val);
    //     break;
    default:
        return QVariant();
    }
}

bool Bulbs::insertRow(int row, const QModelIndex & parent) {
    beginInsertRows(parent, row, row);

    Lightbulb bulb(this, "");
    m_data.insert(row, &bulb);

    endInsertRows();
    return true;
}
