#include "match.h"

Match::Match(std::string result, std::string white, std::string black, int priority, std::string position):
    TournamentsItem(0,0),
    result(result),
    deep(Evaluation()),
    quick(Evaluation()),
    white(white),
    black(black),
    priority(priority),
    position(position)

{

}


int Match::columnCount() const
{
}

QVariant Match::data(int column) const
{
}
