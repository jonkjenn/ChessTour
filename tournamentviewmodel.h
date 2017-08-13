#ifndef TOURNAMENTVIEWMODEL_H
#define TOURNAMENTVIEWMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class TournamentViewModel : public QSortFilterProxyModel
{
    Q_OBJECT
private:
    int tournamentId;
public:
    TournamentViewModel(QObject *parent=Q_NULLPTR,int tournamentId=0);

    // QAbstractItemModel interface
public:
    QHash<int, QByteArray> roleNames() const override;

    // QAbstractProxyModel interface
public:
    //QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    //QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
};

#endif // TOURNAMENTVIEWMODEL_H
