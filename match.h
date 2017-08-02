#ifndef MATCH_H
#define MATCH_H

#include <string>
#include "tournamentsitem.h"
#include "evaluation.h"

class Match: public TournamentsItem
{
public:
    Match(std::string result = "*",std::string white = "White",std::string black = "Black", int priority = 1, std::string position = "");

    std::string result;
    Evaluation deep;
    Evaluation quick;
    std::string white;
    std::string black;
    int priority;
    std::string position;

    // TournamentsItem interface
public:
    int columnCount() const override;
    QVariant data(int column) const override;
};

#endif // MATCH_H
