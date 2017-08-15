import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQml.Models 2.2
import jonkjenn.backend 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Chess tournament viewer")
    objectName: "appWindow"

    ColumnLayout{
        ListView{
            id: tournametView
            model: tourModel
            width: 300
            height: 400
            delegate: Text{text:name}
        }
    }


    Rectangle{
        width: 200
        id: loginDialog
        visible: !Backend.loggedin
        anchors.centerIn: parent

        ColumnLayout{
            /*Label{
                text: "Username"
            }*/
            TextField{
                implicitWidth: 250
                id: username
                KeyNavigation.tab: password
                //text: Backend.username
                focus: true
                placeholderText: "Enter username/email"
            }
            Label{
                text: "Password"
            }

            TextField{
                implicitWidth: 250
                id: password
                echoMode: TextField.PasswordEchoOnEdit
                KeyNavigation.tab: username
                //text: Backend.password
                placeholderText: "Enter password"
            }
            RowLayout{
                Button{
                    text: "Login"
                    onClicked:Backend.login(username.text,password.text)
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
