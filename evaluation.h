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
    std::string movePrevious;
    std::string moveCurrent;
};

#endif // EVALUATION_H
