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
    QString name;
    bool subscribed = false;
    int originalOrder;

    enum class TournamentRoles{
        NameRole = Qt::UserRole+1,
        OriginalOrderRole
    };

    // TournamentsItem interface
    int columnCount() const override;
    QVariant data(int role) const override;
    TournamentsItem *child(int position) const override;
    int childCount() const override;
    bool addChild(TournamentsItem *) override;
    int position(TournamentsItem *) override;

private:
    QVector<Round*> rounds;

};

#endif // TOURNAMENT_H
