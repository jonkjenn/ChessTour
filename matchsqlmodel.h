#ifndef MATCHSQLMODEL_H
#define MATCHSQLMODEL_H
#include <QtSql/QSqlRelationalTableModel>
#include <QHash>

class MatchSqlModel:public QSqlRelationalTableModel
{
public:
    MatchSqlModel(QObject *parent);

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
    bool select() override;
    int getRow(int primaryKey);
private:
    QHash<int,int> rowFromPrimary;
};

#endif // MATCHSQLMODEL_H
