class LightbulbsListModel : public QAbstractListModel {

    Q_OBJECT

public:

    explicit LightbulbsListModel(const std::vector<Lightbulb*>& bulbs, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    std::vector<Lightbulb*> m_bulbs;

};
