#include "Lightbulbs.h"

LightbulbsListModel::LightbulbsListModel(const std::vector<Lightbulb*>& bulbs, QObject *parent) : QAbstractListModel(parent) {
    m_bulbs = bulbs;
}

int LightbulbsListModel::rowCount(const QModelIndex& parent) const {
    return m_bulbs.size();
}

QVariant LightbulbsListModel::data(const QModelIndex& index, int role) const {
    // Check that the index is valid and within the correct range first:
    if (!index.isValid()) return QVariant();
    if (index.row() >= decks_.size()) return QVariant();

    if (role == Qt::DisplayRole) {
        QString bulbName = QString::fromStString(m_bulbs.at(index.row())->label);
        return QVariant(bulbName);
    } else {
        return QVariant();
    }
}
