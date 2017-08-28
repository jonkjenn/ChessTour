#ifndef TOURNAMENTVIEWMODEL_H
#define TOURNAMENTVIEWMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class TournamentViewModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int currentTournament READ currentTournament WRITE setCurrentTournament NOTIFY currentTournamentChanged)
private:
    bool onlySubscribed;
    int m_currenTournament;
public:
    TournamentViewModel(QObject *parent=Q_NULLPTR,bool onlySubscribed = false);

    // QAbstractItemModel interface
    QHash<int, QByteArray> roleNames() const override;
    void setCurrentTournament(int);
    int currentTournament();

    Q_INVOKABLE QModelIndex createChildIndex(int row);

signals:
    void currentTournamentChanged(int);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif // TOURNAMENTVIEWMODEL_H
