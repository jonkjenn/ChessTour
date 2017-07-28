#ifndef EVALUATION_H
#define EVALUATION_H

#include <QString>


class Evaluation
{
public:
    Evaluation();
    float previous;
    float current;
    int depth;
    float change;
    QString movePrevious;
    QString moveCurrent;
};

#endif // EVALUATION_H
