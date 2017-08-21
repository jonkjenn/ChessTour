#ifndef TOURNAMENTVIEWMODEL_H
#define TOURNAMENTVIEWMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class TournamentViewModel : public QSortFilterProxyModel
{
    Q_OBJECT
private:
    bool onlySubscribed;
public:
    TournamentViewModel(QObject *parent=Q_NULLPTR,bool onlySubscribed = false);

    // QAbstractItemModel interface
public:
    QHash<int, QByteArray> roleNames() const override;

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif // TOURNAMENTVIEWMODEL_H
