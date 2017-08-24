#ifndef ROUNDVIEWMODEL_H
#define ROUNDVIEWMODEL_H

#include <QSortFilterProxyModel>


class RoundViewModel:public QSortFilterProxyModel
{
public:
    RoundViewModel(QObject *);

    // QAbstractItemModel interface
public:
    QHash<int, QByteArray> roleNames() const override;
};

#endif // ROUNDVIEWMODEL_H
