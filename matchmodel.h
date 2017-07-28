#ifndef MATCHMODEL_H
#define MATCHMODEL_H

#include <QByteArray>
#include <QHash>

class MatchModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MatchRoles{
        ResultRole,
        DeepRole,
        QuickRole,
        WhiteRole,
        BlackRole,
        PriorityRole,
        PositionRole
    };

    MatchModel(QObject *parent=0);

};

#endif // MATCHMODEL_H
