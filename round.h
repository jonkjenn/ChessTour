#ifndef ROUND_H
#define ROUND_H
#include "match.h"
#include "tournamentsitem.h"
#include <QString>

class Round : public TournamentsItem
{
public:
    Round(TournamentsItem *parent);
    int number;
    QString date;

    enum RoundRoles{
        NumberRole = Qt::UserRole +1,
        DateRole
    };

    // TournamentsItem interface
    QVariant data(int role) const override;

    // TournamentsItem interface
public:
    TournamentsItem *child(int position) const override;
    int childCount() const override;
    bool addChild(TournamentsItem *) override;
};

#endif // ROUND_H
