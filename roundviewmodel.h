#ifndef ROUNDVIEWMODEL_H
#define ROUNDVIEWMODEL_H

#include <QSortFilterProxyModel>
#include <QPersistentModelIndex>


class RoundViewModel:public QSortFilterProxyModel
{
    Q_OBJECT

public:
    RoundViewModel(QObject *);

    // QAbstractItemModel interface
public:
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QModelIndex createChildIndex(int parentRow, int parentParentRow);
};

#endif // ROUNDVIEWMODEL_H
