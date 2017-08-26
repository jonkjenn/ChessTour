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
    Match(TournamentsItem *parent,int id);

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
        QuickCurrentMoveRole,

        FullGameRKeyRole,
        IdRole
    };

    int id;
    // TournamentsItem interface
    int columnCount() const override;
    QVariant data(int column) const override;
    int childCount() const override;
    bool addChild(TournamentsItem *) override;
    TournamentsItem *child(int position) const override;
    int position(TournamentsItem *) override;
    bool setData(int role, const QVariant &value) override;

private:
    QString resultWhite;
    QString resultBlack;

    Evaluation deep;
    Evaluation quick;

    QString white;
    int whiteFide;
    QString black;
    int blackFide;

    int priority;
    QString gamePosition;
    QString fullGameRKey;

    QString currentFEN;
    QString earlierFEN;
    QStringList lastMoves;

    QString engine;

};

#endif // MATCH_H
