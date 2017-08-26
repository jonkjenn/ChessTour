#include "matchViewModel.h"
#include "match.h"

QHash<int,QByteArray> MatchViewModel::roleNames() const{
    QHash<int, QByteArray> roles;

    roles.insert(static_cast<int>(Match::MatchRoles::ResultWhiteRole),"resultWhiteRole");
    roles.insert(static_cast<int>(Match::MatchRoles::ResultBlackRole),"resultBlackRole");
    roles.insert(static_cast<int>(Match::MatchRoles::WhiteRole),"whiteRole");
    roles.insert(static_cast<int>(Match::MatchRoles::BlackRole),"blackRole");
    roles.insert(static_cast<int>(Match::MatchRoles::WhiteFideRole),"whiteFideRole");
    roles.insert(static_cast<int>(Match::MatchRoles::BlackFideRole),"blackFideRole");
    roles.insert(static_cast<int>(Match::MatchRoles::PriorityRole),"priorityRole");
    roles.insert(static_cast<int>(Match::MatchRoles::PositionRole),"positionRole");
    roles.insert(static_cast<int>(Match::MatchRoles::DeepPreviousRole),"deepPreviousRole");
    roles.insert(static_cast<int>(Match::MatchRoles::DeepCurrentRole),"deepCurrentRole");
    roles.insert(static_cast<int>(Match::MatchRoles::DeepDepthRole),"deepDepthRole");
    roles.insert(static_cast<int>(Match::MatchRoles::DeepChangeRole),"deepChangeRole");
    roles.insert(static_cast<int>(Match::MatchRoles::DeepPreviousMoveRole),"deepPreviousMoveRole");
    roles.insert(static_cast<int>(Match::MatchRoles::DeepCurrentMoveRole),"deepCurrentMoveRole");
    roles.insert(static_cast<int>(Match::MatchRoles::QuickPreviousRole),"quickPreviousRole");
    roles.insert(static_cast<int>(Match::MatchRoles::QuickCurrentRole),"quickCurrentRole");
    roles.insert(static_cast<int>(Match::MatchRoles::QuickDepthRole),"quickDepthRole");
    roles.insert(static_cast<int>(Match::MatchRoles::QuickChangeRole),"quickChangeRole");
    roles.insert(static_cast<int>(Match::MatchRoles::QuickPreviousMoveRole),"quickPreviousMoveRole");
    roles.insert(static_cast<int>(Match::MatchRoles::QuickCurrentMoveRole),"quickCurrentMoveRole");
    roles.insert(static_cast<int>(Match::MatchRoles::FullGameRKeyRole),"fullGameRKeyRole");
    roles.insert(static_cast<int>(Match::MatchRoles::IdRole),"idRole");

    return roles;
}

MatchViewModel::MatchViewModel(QObject *parent):
    QSortFilterProxyModel(parent)
{

}
