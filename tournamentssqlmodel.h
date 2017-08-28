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
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    void forceRefresh();

    int getRow(int primaryKey);
    int getPk(int row);

    Q_INVOKABLE void setCurrentIndex(int row);

signals:
    void currentIndexChanged(int row);

private:
    QSqlDatabase database;
    QHash<int,int> rowFromPrimary;

    // QSqlTableModel interface
public slots:
    bool select() override;
};

#endif // TOURNAMENTSSQLMODEL_H
