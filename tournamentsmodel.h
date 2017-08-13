#ifndef TOURNAMENTSMODEL_H
#define TOURNAMENTSMODEL_H

#include <QAbstractItemModel>

//Forward declarations
class TournamentsItem;

class TournamentsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TournamentsModel(QObject *parent=0);
    ~TournamentsModel();

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    bool insertColumns(int column, int count, const QModelIndex &parent);
    bool removeColumns(int column, int count, const QModelIndex &parent);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool hasChildren(const QModelIndex &parent) const override;

    void httpFinished();
    TournamentsItem *rootItem = nullptr;

    // QAbstractItemModel interface

    // QAbstractItemModel interface
};

#endif // TOURNAMENTSMODEL_H
