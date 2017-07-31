#ifndef EVALUATION_H
#define EVALUATION_H
#include <string>

class Evaluation
{
public:
    Evaluation();
    float previous;
    float current;
    int depth;
    float change;
    string movePrevious;
    string moveCurrent;
};

#endif // EVALUATION_H
