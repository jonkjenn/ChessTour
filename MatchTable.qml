import QtQuick 2.0
import QtQuick.Controls 1.4

TableView{
    model:matchSqlModel

    TableViewColumn{
        role: "players_name_2"
        title: "White"
    }
    TableViewColumn{
        role: "name"
        title: "Black"
    }
    TableViewColumn{
        role: "resultwhite"
        title: "Score"
        width: 15
    }
    TableViewColumn{
        role: "resultblack"
        title: "Score"
        width: 15
    }
    TableViewColumn{
        id:enginescore
        role: "enginescore"
        title: "Engine"
        width: 80
    }
    TableViewColumn{
        id:enginemate
        role: "enginemate"
        title: "Mate?"
        width: 80
    }
    TableViewColumn{
        role: "status"
        title: "Status"
        width: 80
    }

    Connections{
        target:roundsView
        onCurrentRoundPKChanged:{
                matchSqlModel.setRound(roundsView.currentRoundPK)
        }
    }
}
