import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQml.Models 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Styles 1.4
import QtMultimedia 5.9

ApplicationWindow {
    visible: true
    title: qsTr("Chess tournament viewer")
    objectName: "appWindow"
    Material.foreground: "White"

    Audio {
        id: playMusic
        source: "qrc:///ding.wav"
        volume: volumeSlider.volume
    }

    RowLayout{
        anchors.fill: parent
        anchors.margins: 10
        implicitWidth: 1024
        implicitHeight: 512

        Item{
            visible: false
            id:showButton
            Layout.maximumWidth: 20
            width: 20
            Button{
                Layout.margins: 0
                implicitHeight: 40
                anchors.verticalCenter: parent.verticalCenter
                x: -implicitWidth/2
                rotation: 90
                text:qsTr("Show")
                onClicked: {tournamentColumn.visible = true;showButton.visible = false;}
            }
        }

        ColumnLayout{
            id:tournamentColumn
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

            RowLayout{
                Layout.fillWidth: true
                Layout.preferredWidth: tournamentList.width
                implicitHeight: 40

                Button{
                    implicitHeight: parent.height
                    implicitWidth: 90
                    id: control
                    text: qsTr("Refresh")
                    onClicked:tournamentController.refreshTournamentList()
                    opacity: tournamentController.canRefreshTournamentList?1:0.3
                }

                Button{
                    implicitHeight: parent.height
                    implicitWidth: 90
                    id: hideButton
                    text: qsTr("Hide")
                    onClicked:{tournamentColumn.visible = false;showButton.visible = true;}
                    anchors.right: parent.right
                }
            }

            TournamentList{
                id: tournamentList
                Layout.fillHeight: true
                Layout.preferredWidth: 250
            }
        }
        Rectangle{
            color:"white"
            width: 2
            Layout.fillHeight: true
            height: parent.height
        }
        ColumnLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true
            //property var currentIndex
            id:tournamentView

            Label{
                id:tournamentNameHeader
                height: 35
                width: Layout.fillWidth
            }

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
                    id:volumeOffButton
                            width: 32
                            height: 32
                        background: Image{
                            id:volumeButtonImage
                            source: volumeSlider.volume>0?"qrc:///speaker.png":"qrc:///speakeroff.png";
                            sourceSize.height: 32
                            sourceSize.width: 32
                        }
                        onClicked: if(volumeSlider.volume>0){
                                       volumeSlider.previousVolume = volumeSlider.value;
                                       volumeSlider.value=0;
                                   }else{
                                       volumeSlider.value = volumeSlider.previousVolume;
                                   }
                    }


                    Slider {
                        anchors.verticalCenter: volumeOffButton.verticalCenter
                        id: volumeSlider
                        implicitWidth: 75
                        implicitHeight: 32
                        property real volume: QtMultimedia.convertVolume(volumeSlider.value,
                                                              QtMultimedia.LogarithmicVolumeScale,
                                                                         QtMultimedia.LinearVolumeScale)
                        property real previousVolume: 0.1
                    }
                Button{
                    implicitHeight: 50
                    implicitWidth: 90
                    id: button
                    text: qsTr("Refresh")
                    onClicked:tournamentController.refreshTournament(tournamentList.currentIndex)
                    opacity: tournamentController.canRefreshTournament?1:0.3
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
                    text: "Games: "
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
                    gameSelectModel.clear();
                    for(var i=0;i<matchSqlModel.gamesPerMatch();++i){
                        gameSelectModel.append({"gameNumber":i+1});
                    }
                    gameSelectList.currentIndex = matchSqlModel.currentGameNumber()-1
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
                    placeholderText: "Chess24.com username/email"
                }

                TextField{
                    implicitWidth: 250
                    id: password
                    echoMode: TextField.PasswordEchoOnEdit | null
                    KeyNavigation.tab: username
                    placeholderText: "Chess24.com password"
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
