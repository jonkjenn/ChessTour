#ifndef ROUNDVIEWMODEL_H
#define ROUNDVIEWMODEL_H

#include <QSortFilterProxyModel>


class RoundViewModel:public QSortFilterProxyModel
{
public:
    RoundViewModel(QObject *);
};

#endif // ROUNDVIEWMODEL_H
