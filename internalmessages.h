#ifndef INTERNALMESSAGES_H
#define INTERNALMESSAGES_H

#include <QString>

namespace InternalMessages{
struct TournamentChangedData{
    TournamentChangedData(int pk, int currentRoundIndex, QString name, QString eventType, bool subscribed):pk(pk),currentRoundIndex(currentRoundIndex),name(name),eventType(eventType),subscribed(subscribed){}
    const int pk;
    const int currentRoundIndex = 1;
    int currentGame = 1;
    const bool subscribed;
    const QString name;
    const QString eventType;
};
struct RoundChangedData{
    RoundChangedData(int pk, int gamesPerMatch = 1):pk(pk),gamesPerMatch(gamesPerMatch){}
    const int pk;
    const int gamesPerMatch;
};
}

#endif // INTERNALMESSAGES_H
