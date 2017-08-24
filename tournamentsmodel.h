#ifndef TOURNAMENTSMODEL_H
#define TOURNAMENTSMODEL_H

#include <QAbstractItemModel>

#include "tournamentsitem.h"
#include "tournament.h"
#include "rootitem.h"

class Chess24Manager;

class TournamentsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TournamentsModel(QObject *parent, Chess24Manager &);
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
    Chess24Manager &c24Manager;


    bool addChild(const QModelIndex &parent,TournamentsItem *);
    bool addTournament(QString name, int originalOrder);
    bool addTournaments(QVariantList names);
    void updateTournamentDetails(QJsonObject json);
    void onCurrentTournamentChanged(QModelIndex index);
};

#endif // TOURNAMENTSMODEL_H
