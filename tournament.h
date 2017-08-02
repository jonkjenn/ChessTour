#ifndef TOURNAMENT_H
#define TOURNAMENT_H
#include <string>
#include <vector>
#include "round.h"
#include "tournamentsitem.h"

class Tournament: public TournamentsItem
{
public:
    ~Tournament();
    Tournament(std::string name = "Unnamed Tournament");
    std::string name;
    int columnCount() const override;
};

#endif // TOURNAMENT_H
