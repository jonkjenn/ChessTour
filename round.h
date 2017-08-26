#ifndef ROUND_H
#define ROUND_H
#include "match.h"
#include "tournamentsitem.h"
#include <QString>

class Round : public TournamentsItem
{
public:
    Round(TournamentsItem *parent, int number);
    int number;
    QString date;
    QString description;

    enum class RoundRoles{
        NumberRole = Qt::UserRole +1,
        DateRole,
        DescriptionRole
    };

    // TournamentsItem interface
    QVariant data(int role) const override;

    // TournamentsItem interface
    TournamentsItem *child(int position) const override;
    int childCount() const override;
    int columnCount() const override;
    bool addChild(TournamentsItem *) override;
    int position(TournamentsItem *) override;
    int position(int id);
    bool setData(int role, const QVariant &value) override;

private:
    QVector<Match*> matches;

};

#endif // ROUND_H
