import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQml.Models 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    visible: true
    title: qsTr("Chess tournament viewer")
    objectName: "appWindow"
    Material.foreground: "White"

    RowLayout{
        anchors.fill: parent
        anchors.margins: 10
        implicitWidth: 1024
        implicitHeight: 512
        ColumnLayout{
            Layout.fillHeight: true
            Item{
                id: tournmanetListHeader
                Layout.preferredHeight: 30
                Layout.preferredWidth: tournamentList.width
                Label{
                    anchors.centerIn: tournmanetListHeader
                    text: "Tournaments";
                    font.bold: true
                }
            }
            Button{
                anchors.horizontalCenter: parent.horizontalCenter
                implicitHeight: 50
                implicitWidth: 90
                id: control
                text: qsTr("Refresh")
                onClicked:c24Manager.refreshTournamentList()
                opacity: c24Manager.canRefreshTournamentList?1:0.3
            }

            TournamentList{
                id: tournamentList
                Layout.fillHeight: true
                Layout.preferredWidth: 250
            }
        }
        ColumnLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true
            //property var currentIndex
            id:tournamentView

            RowLayout{
                id:roundRow
                height: 35
                Label{
                    anchors.verticalCenter: roundRow.verticalCenter
                    text: "Rounds:"
                }

                RoundList{
                    id:roundsView
                    Layout.fillWidth: true
                    height: roundRow.height
                    anchors.verticalCenter: roundRow.verticalCenter
                }
                Button{
                    implicitHeight: 50
                    implicitWidth: 90
                    id: button
                    text: qsTr("Refresh")
                    onClicked:c24Manager.refreshTournament(tournamentList.currentIndex)
                    opacity: c24Manager.canRefreshTournament?1:0.3
                }
            }

            /*MatchTable{
                Layout.fillWidth: true
                Layout.fillHeight: true
            }*/

            ListModel{
                id: gameSelectModel
            }

            RowLayout{
                id:gameSelectView
                visible: false

                Label{
                    anchors.verticalCenter: gameSelectView.verticalCenter
                    text: "Game: "
                }

            ListView{
                id:gameSelectList
                orientation: ListView.Horizontal
                Layout.fillWidth: true
                model:gameSelectModel
                height: 30

                delegate: ItemDelegate{
                    id: gameSelectControl
                    height: gameSelectList.height
                    width: 30

                    contentItem:
                                Label{
                                    text: gameNumber
                                    font.pixelSize: 20
                                    verticalAlignment: Label.AlignVCenter
                                    horizontalAlignment: Label.AlignHCenter
                                    }

                    background: Rectangle{
                        color: if(index === gameSelectList.currentIndex){
                                   return Material.color(Material.Amber);
                               }else{
                                   if(gameSelectMA.containsMouse){
                                   return Material.color(Material.Grey)
                                   }
                                   return "transparent"
                               }
                        border.width: 2
                        border.color:
                            if(matchSqlModel.currentGameNumber()===index+1
                                    && control.highlighted
                                    ){
                                  return "#7af442";
                              }else{
                                  return "transparent";
                              }
                    }

        MouseArea {
            id:gameSelectMA
            anchors.fill: parent
            onClicked: {
                        matchSqlModel.setCurrentGameNumber(gameNumber)
                        gameSelectList.currentIndex = index
            }
            hoverEnabled: true
        }
                }
            }
            }

            MatchList{
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Connections{
                target:matchSqlModel
                onCurrentRoundLoaded:{
                    gameSelectView.visible = matchSqlModel.eventType() === "knockout"
                    console.log(matchSqlModel.eventType());
                    gameSelectModel.clear();
                    for(var i=0;i<matchSqlModel.gamesPerMatch();++i){
                        gameSelectModel.append({"gameNumber":i+1});
                    }
                }

            }
        }
    }

    Dialog{
        visible: !chess24Login.loggedin
        id: loginWrapper
        contentItem:Rectangle{
            color:Material.background
            implicitWidth: 300
            implicitHeight: 150
            id: loginDialog

            ColumnLayout{
                anchors.margins: 10
                anchors.centerIn: loginDialog
                TextField{
                    implicitWidth: 250
                    id: username
                    KeyNavigation.tab: password
                    focus: true
                    placeholderText: "Enter username/email"
                }

                TextField{
                    implicitWidth: 250
                    id: password
                    echoMode: TextField.PasswordEchoOnEdit | null
                    KeyNavigation.tab: username
                    placeholderText: "Enter password"
                }
                RowLayout{
                    Button{
                        text: "Login"
                        onClicked:chess24Login.login(username.text,password.text)
                    }
                    Button{
                        text:"Cancel"
                        onClicked: loginWrapper.visible = false
                    }
                }
            }

            onVisibleChanged: if(visible){
                                  username.forceActiveFocus()
                              }

        }
    }

}
