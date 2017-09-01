#ifndef LIVEMATCHSQLMODEL_H
#define LIVEMATCHSQLMODEL_H
#include <QAbstractListModel>
#include <QtSql/QSqlDatabase>

class LiveMatchSqlModel:public QAbstractListModel
{
    Q_OBJECT
public:
    LiveMatchSqlModel(QObject *parent, QSqlDatabase &database);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(int row, QString column, const QVariant &value);
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setRound(int pk);
    int currentPk();
private:
    QSqlDatabase &database;
    int m_rowCount = 0;
    int m_currentPk = -1;
    QHash<int, QByteArray> roles;
    QHash<int, QString> roleIdToColumn;
    QHash<QString, int> columnToRoleId;
    QHash<int,int> rowToPk;
};

#endif // LIVEMATCHSQLMODEL_H
