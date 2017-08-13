#ifndef TOURNAMENT_H
#define TOURNAMENT_H
#include <QString>
#include <QVariant>
#include "round.h"
#include "tournamentsitem.h"

class Tournament: public TournamentsItem
{
public:
    Tournament(TournamentsItem *parent,QString name = "Unnamed Tournament");
    QString name;
    int columnCount() const override;
    enum TournamentRoles{
        NameRole = Qt::UserRole+1
    };
    QVariant data(int role) const override;
};

#endif // TOURNAMENT_H
