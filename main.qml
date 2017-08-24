import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQml.Models 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.2

ApplicationWindow {
    visible: true
    title: qsTr("Chess tournament viewer")
    objectName: "appWindow"

    RowLayout{
        implicitWidth: 1024
        implicitHeight: 512
    ColumnLayout{
            Layout.fillHeight: true
            implicitWidth: 150
        Text{ id: tournamentHeader;text: "TournamentView"}
        TreeView{
            id: tournamentsList
            model: tournamentsModel
            Layout.fillHeight: true
            selectionMode: SelectionMode.SingleSelection

            TableViewColumn {
                title: "Name"
                role: "name"
            }
            rowDelegate: Rectangle{
                color: (styleData.selected?Material.color(Material.Orange):Material.color(Material.LightBlue))
            }
        }
    }
    ColumnLayout{
        Layout.fillWidth: true
        implicitWidth: 1024
        property var currentIndex
        id:tournamentView
        TreeView{
            id: roundsView
            model: roundsModel
            rootIndex: roundsModel.mapFromSource(tournamentsModel.mapToSource(tournamentsList.currentIndex))
            visible:tournamentsList.currentIndex.valid

            TableViewColumn {
                title: "Round"
                role: "number"
            }
            TableViewColumn {
                title: "Date"
                role: "date"
            }
        }
        TreeView{
            id: matchesView
            model: matchesModel
            rootIndex: matchesModel.mapFromSource(roundsModel.mapToSource(roundsView.currentIndex))
            visible:roundsView.currentIndex.valid

            TableViewColumn{
                title:"White"
                role:"whiteRole"
            }

            TableViewColumn{
                title:"Black"
                role:"blackRole"
            }
        }
    }
    }

    Dialog{
        visible: !chess24Login.loggedin
        id: loginWrapper
    contentItem:Rectangle{
        color: Material.background
        implicitWidth: 300
        implicitHeight: 150
        id: loginDialog

        ColumnLayout{
            anchors.margins: 10
            anchors.centerIn: loginDialog
            TextField{
                textColor: Material.foreground
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
