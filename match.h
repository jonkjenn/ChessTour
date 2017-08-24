#ifndef MATCH_H
#define MATCH_H

#include <QVariant>
#include <QString>
#include "tournamentsitem.h"
#include "evaluation.h"

class Round;
class Tournament;

class Match: public TournamentsItem
{
public:
    Match(TournamentsItem *parent,const Round &, const Tournament &);

    QString resultWhite;
    QString resultBlack;

    Evaluation deep;
    Evaluation quick;

    QString white;
    int whiteFide;
    QString black;
    int blackFide;

    int priority;
    QString position;
    QString fullGameRKey;

    QString currentFEN;
    QString earlierFEN;
    QStringList lastMoves;

    QString engine;

    const Round &round;
    const Tournament &tournament;

    enum class MatchRoles{
        ResultWhiteRole = Qt::UserRole +1,
        ResultBlackRole,
        WhiteRole,
        BlackRole,
        WhiteFideRole,
        BlackFideRole,
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

    // TournamentsItem interface
public:
    //TournamentsItem *child(int position)
    int childCount() const override;
    bool addChild(TournamentsItem *) override;
};

#endif // MATCH_H
