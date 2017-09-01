#ifndef ROUNDSSQLMODEL_H
#define ROUNDSSQLMODEL_H
#include <QtSql/QSqlTableModel>
#include <QHash>
#include <QSqlDatabase>

class RoundsSqlModel:public QSqlTableModel
{
    Q_OBJECT
    Q_SIGNAL void roundDataChanged();
public:
    RoundsSqlModel(QObject *parent, QSqlDatabase database);

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool select() override;

    Q_INVOKABLE int getRow(int primaryKey);
    Q_INVOKABLE int getPk(int row);
    Q_INVOKABLE int currentShownRound();
    Q_INVOKABLE void setCurrentIndex(int index);
    Q_INVOKABLE void tournamentChanged(int pk);

    void setCurrentShownRound(int currentShownRound);
    void forceRefresh();
    int currentPK();

private:
    QHash<int,int>rowFromPrimary;
    QSqlDatabase database;
    int m_currentPK = -1;
};

#endif // ROUNDSSQLMODEL_H
