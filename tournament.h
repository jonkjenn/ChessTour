#ifndef TOURNAMENT_H
#define TOURNAMENT_H
#include <QString>
#include <QVariant>
#include "round.h"
#include "tournamentsitem.h"

class Tournament: public TournamentsItem
{
public:
    Tournament(TournamentsItem *parent,QString name = "Unnamed Tournament",int originalOrder = -1);
    bool subscribed = false;

    enum class TournamentRoles{
        NameRole = Qt::UserRole+1,
        OriginalOrderRole,
        StatusRole,
        EventTypeRole,
        CurrentShownRoundRole
    };
    QString name;

    // TournamentsItem interface
    int columnCount() const override;
    QVariant data(int role) const override;
    QVariant data(Tournament::TournamentRoles role) const;
    TournamentsItem *child(int position) const override;
    int childCount() const override;
    bool addChild(TournamentsItem *) override;
    int position(TournamentsItem *) override;
    int position(int id);

    QHash<int,QString> players;

private:
    QVector<Round*> rounds;
    int originalOrder;
    QString status;
    QString eventType;
    int currentShownRound = -1;

    // TournamentsItem interface
public:
    bool setData(int role, const QVariant &value) override;
};

#endif // TOURNAMENT_H
