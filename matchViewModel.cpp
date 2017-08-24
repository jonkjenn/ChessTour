#include "matchViewModel.h"
#include "match.h"

QHash<int,QByteArray> MatchViewModel::roleNames() const{
    QHash<int, QByteArray> roles;

    roles[static_cast<int>(Match::MatchRoles::ResultWhiteRole)] =          "resultWhiteRole";
    roles[static_cast<int>(Match::MatchRoles::ResultBlackRole)] =          "resultBlackRole";
    roles[static_cast<int>(Match::MatchRoles::WhiteRole)] =                "whiteRole";
    roles[static_cast<int>(Match::MatchRoles::BlackRole)] =                "blackRole";
    roles[static_cast<int>(Match::MatchRoles::WhiteFideRole)] =            "whiteFideRole";
    roles[static_cast<int>(Match::MatchRoles::BlackFideRole)] =            "blackFideRole";
    roles[static_cast<int>(Match::MatchRoles::PriorityRole)] =             "priorityRole";
    roles[static_cast<int>(Match::MatchRoles::PositionRole)] =             "positionRole";
    roles[static_cast<int>(Match::MatchRoles::DeepPreviousRole)] =         "deepPreviousRole";
    roles[static_cast<int>(Match::MatchRoles::DeepCurrentRole)] =          "deepCurrentRole";
    roles[static_cast<int>(Match::MatchRoles::DeepDepthRole)] =            "deepDepthRole";
    roles[static_cast<int>(Match::MatchRoles::DeepChangeRole)] =           "deepChangeRole";
    roles[static_cast<int>(Match::MatchRoles::DeepPreviousMoveRole)] =     "deepPreviousMoveRole";
    roles[static_cast<int>(Match::MatchRoles::DeepCurrentMoveRole)] =      "deepCurrentMoveRole";
    roles[static_cast<int>(Match::MatchRoles::QuickPreviousRole)] =        "quickPreviousRole";
    roles[static_cast<int>(Match::MatchRoles::QuickCurrentRole)] =         "quickCurrentRole";
    roles[static_cast<int>(Match::MatchRoles::QuickDepthRole)] =           "quickDepthRole";
    roles[static_cast<int>(Match::MatchRoles::QuickChangeRole)] =          "quickChangeRole";
    roles[static_cast<int>(Match::MatchRoles::QuickPreviousMoveRole)] =    "quickPreviousMoveRole";
    roles[static_cast<int>(Match::MatchRoles::QuickCurrentMoveRole)] =     "quickCurrentMoveRole";

    return roles;
}

MatchViewModel::MatchViewModel(QObject *parent)
{

}
