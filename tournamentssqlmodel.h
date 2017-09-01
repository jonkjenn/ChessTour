#ifndef TOURNAMENTSSQLMODEL_H
#define TOURNAMENTSSQLMODEL_H
#include <QtSql/QSqlTableModel>
#include <QHash>

class TournamentsSqlModel:public QSqlTableModel
{
    Q_OBJECT
public:
    TournamentsSqlModel(QObject *parent,QSqlDatabase database);

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant data(int row, QString columnName) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    void forceRefresh();

    int getRow(int primaryKey);
    Q_INVOKABLE int getPk(int row);
    Q_INVOKABLE void setCurrentIndex(int row);

    bool setData(int row, QString column, const QVariant &value);
signals:
    void currentIndexChanged(int row);
    void currentPKChanged(int pk);

private:
    QSqlDatabase database;
    QHash<int,int> rowFromPrimary;

    // QSqlTableModel interface
public slots:
    bool select() override;
};

#endif // TOURNAMENTSSQLMODEL_H
