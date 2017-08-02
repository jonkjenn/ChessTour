#include "tournament.h"

Tournament::~Tournament()
{

}

Tournament::Tournament(std::string name):name(name)
{

}

int Tournament::columnCount() const
{
    return 1;
}
