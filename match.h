#ifndef MATCH_H
#define MATCH_H

#include "evaluation.h"
#include <string>

class Match
{
public:
    Match(string result = "*",string white = "White",string black = "Black", int priority = 1, string position = "");
    string result;
    Evaluation deep;
    Evaluation quick;
    string white;
    string black;
    int priority;
    string position;
};

#endif // MATCH_H
