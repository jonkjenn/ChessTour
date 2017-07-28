#ifndef MATCH_H
#define MATCH_H

#include "evaluation.h"

class Match
{
public:
    Match(QString result = "*",QString white = "White",QString black = "Black", int priority = 1, QString position = "");
    QString result;
    Evaluation deep;
    Evaluation quick;
    QString white;
    QString black;
    int priority;
    QString position;
};

#endif // MATCH_H
