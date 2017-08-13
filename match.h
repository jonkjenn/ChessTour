#ifndef MATCH_H
#define MATCH_H

#include <QVariant>
#include <QString>
#include "tournamentsitem.h"
#include "evaluation.h"

class Match: public TournamentsItem
{
public:
    Match(TournamentsItem *parent, QString result = "*",QString white = "White",QString black = "Black", int priority = 1, QString position = "");

    QString result;
    Evaluation deep;
    Evaluation quick;
    QString white;
    QString black;
    int priority;
    QString position;

    enum MatchRoles{
        ResultRole = Qt::UserRole +1,
        WhiteRole,
        BlackRole,
        PriorityRole,
        PositionRole,

        DeepPreviousRole,
        DeepCurrentRole,
        DeepDepthRole,
        DeepChangeRole,
        DeepPreviousMoveRole,
        DeepCurrentMoveRole,

        QuickPreviousRole,
        QuickCurrentRole,
        QuickDepthRole,
        QuickChangeRole,
        QuickPreviousMoveRole,
        QuickCurrentMoveRole
    };

    // TournamentsItem interface
public:
    int columnCount() const override;
    QVariant data(int column) const override;
};

#endif // MATCH_H
