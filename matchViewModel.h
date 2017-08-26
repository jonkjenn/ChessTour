#ifndef MATCHMODEL_H
#define MATCHMODEL_H

#include <QByteArray>
#include <QHash>
#include <QSortFilterProxyModel>

class MatchViewModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    MatchViewModel(QObject *parent);

    QHash<int, QByteArray> roleNames() const;
};

#endif // MATCHMODEL_H
