#ifndef TOURNAMENT_H
#define TOURNAMENT_H
#include <string>
#include <vector>
#include "round.h"

class Tournament
{
public:
    Tournament(string name = "Unnamed Tournament");
    std::string name;
    vector<Round> rounds;

};

#endif // TOURNAMENT_H
