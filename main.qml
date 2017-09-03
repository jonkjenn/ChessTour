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
                RoundList{
                    id:roundsView
                    Layout.fillWidth: true
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

            MatchList{
                Layout.fillWidth: true
                Layout.fillHeight: true
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
