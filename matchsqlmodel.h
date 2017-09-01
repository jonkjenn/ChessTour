#ifndef MATCHSQLMODEL_H
#define MATCHSQLMODEL_H
#include <QtSql/QSqlRelationalTableModel>
#include <QHash>
#include <QSqlDatabase>

class MatchSqlModel:public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    MatchSqlModel(QObject *parent,QSqlDatabase database);

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant data(int row, QString column) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
    bool select() override;
    int getRow(int primaryKey);

    Q_INVOKABLE void setRound(int pk);
    void forceRefresh();
    bool setData(int row, QString fieldName, const QVariant &value, int role);
    int currentPK();
    bool setDataNoDataChanged(int row, QString fieldName, const QVariant &value);
    void sendDataChanged(int row);
public slots:
    void onRoundChanged(int pk);
private:
    QHash<int,int> rowFromPrimary;
    QSqlDatabase database;
    int m_currentPK = -1;
};

#endif // MATCHSQLMODEL_H
