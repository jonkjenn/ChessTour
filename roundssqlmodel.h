#ifndef ROUNDSSQLMODEL_H
#define ROUNDSSQLMODEL_H
#include <QtSql/QSqlTableModel>
#include <QHash>

class RoundsSqlModel:public QSqlTableModel
{
public:
    RoundsSqlModel(QObject *parent);

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool select() override;
    int getRow(int primaryKey);
private:
    QHash<int,int>rowFromPrimary;
};

#endif // ROUNDSSQLMODEL_H
