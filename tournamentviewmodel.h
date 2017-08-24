#ifndef TOURNAMENTVIEWMODEL_H
#define TOURNAMENTVIEWMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class TournamentViewModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QModelIndex currentTournament READ currentTournament WRITE setCurrentTournament NOTIFY currentTournamentChanged)
private:
    bool onlySubscribed;
    QModelIndex m_currenTournament;
public:
    TournamentViewModel(QObject *parent=Q_NULLPTR,bool onlySubscribed = false);

    // QAbstractItemModel interface
    QHash<int, QByteArray> roleNames() const override;
    void setCurrentTournament(QModelIndex);
    QModelIndex currentTournament();

signals:
    void currentTournamentChanged(QModelIndex);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif // TOURNAMENTVIEWMODEL_H
