#ifndef INTERNALMESSAGES_H
#define INTERNALMESSAGES_H

#include <QMetaType>
#include <QString>

namespace InternalMessages{
struct TournamentChangedData{
    TournamentChangedData(){}
    TournamentChangedData(const TournamentChangedData& data){
        pk = data.pk;
        currentRoundIndex = data.currentRoundIndex;
        currentGame = data.currentGame;
        name = data.name;
        eventType = data.eventType;
        subscribed = data.subscribed;
    }
    ~TournamentChangedData(){}
    TournamentChangedData(int pk, int currentRoundIndex, QString name, QString eventType, bool subscribed):pk(pk),currentRoundIndex(currentRoundIndex),name(name),eventType(eventType),subscribed(subscribed){}
    int pk;
    int currentRoundIndex = 1;
    int currentGame = 1;
    QString name;
    QString eventType;
    bool subscribed;
};}
Q_DECLARE_METATYPE(InternalMessages::TournamentChangedData)

namespace InternalMessages {
struct RoundChangedData{
    RoundChangedData(){}
    RoundChangedData(const RoundChangedData& data){
        pk = data.pk;
        gamesPerMatch = data.gamesPerMatch;
    }
    ~RoundChangedData(){}
    RoundChangedData(int pk, int gamesPerMatch = 1):pk(pk),gamesPerMatch(gamesPerMatch){}
    int pk;
    int gamesPerMatch;
};
}

Q_DECLARE_METATYPE(InternalMessages::RoundChangedData)

#endif // INTERNALMESSAGES_H
