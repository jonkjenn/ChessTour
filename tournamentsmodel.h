#ifndef TOURNAMENTSMODEL_H
#define TOURNAMENTSMODEL_H

#include <QAbstractItemModel>
#include "tournamentsitem.h"
#include "tournament.h"
#include "rootitem.h"


class TournamentsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TournamentsModel(QObject *parent=0);
    ~TournamentsModel();

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool hasChildren(const QModelIndex &parent) const override;

    void httpFinished();
    TournamentsItem *rootItem = nullptr;
    RootItem rootItemObject{nullptr};


    bool addChild(const QModelIndex &parent,TournamentsItem *);
    bool addTournament(QString name, int originalOrder);
    bool addTournaments(QVariantList names);
};

#endif // TOURNAMENTSMODEL_H
