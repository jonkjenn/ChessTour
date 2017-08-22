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
        Text{text: "TournamentView"}
        TreeView{
            id: tournametView
            model: tournamentModel
            Layout.fillHeight: true
            TableViewColumn {
                title: "Name"
                role: "name"
            }
            rowDelegate: Rectangle{
                color: (styleData.selected?Material.color(Material.Orange):Material.color(Material.LightBlue))
            }
        }
    }


    Rectangle{
        width: 200
        id: loginDialog
        visible: !backend.loggedin
        anchors.centerIn: parent

        ColumnLayout{
            /*Label{
                text: "Username"
            }*/
            TextField{
                implicitWidth: 250
                id: username
                KeyNavigation.tab: password
                //text: backend.username
                focus: true
                placeholderText: "Enter username/email"
            }
            Label{
                text: "Password"
            }

            TextField{
                implicitWidth: 250
                id: password
                echoMode: TextField.PasswordEchoOnEdit | null
                KeyNavigation.tab: username
                //text: backend.password
                placeholderText: "Enter password"
            }
            RowLayout{
                Button{
                    text: "Login"
                    onClicked:backend.login(username.text,password.text)
                }
                Button{
                    text:"Cancel"
                    onClicked: loginDialog.visible = false
                }
            }
        }

        onVisibleChanged: if(visible){
                              username.forceActiveFocus()
                          }

    }


}
