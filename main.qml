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
        anchors.fill: parent
        implicitWidth: 1024
        implicitHeight: 512
    ColumnLayout{
            Layout.fillHeight: true
        Rectangle{
            Layout.preferredHeight: 30
            Layout.preferredWidth: tournamentsList.width

            color: Material.color(Material.LightBlue)
            Text{ id: tournamentHeader;text: "Tournaments2:";font.bold: true }
        }
        ListView{
            Layout.fillHeight: true
            Layout.preferredWidth: 250
            id: tournamentsList
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            focus:true
            model: DelegateModel{
                model: tournamentsSqlModel

                delegate:Rectangle{
                    id: rect
                    height: 30
                    width: tournamentsList.width
                    color: ListView.isCurrentItem?Material.color(Material.Orange):Material.color(Material.LightBlue)
                    Text {
                        anchors.verticalCenter: rect.verticalCenter
                        anchors.left: rect.left
                        anchors.leftMargin: 5
                        text:name
                    }
                    MouseArea {
                        anchors.fill: rect
                        onClicked: {
                            console.log("Clicked")
                            tournamentsList.currentIndex = index
                        }
                    }
                }

                //rowDelegate: Rectangle{
                //color: (styleData.selected?Material.color(Material.Orange):Material.color(Material.LightBlue))
            }

            //onClicked: tournamentViewModel.sourceModel.setData(tournamentViewModel.mapToSource(currentIndex),

            onCurrentIndexChanged:{
                tournamentsSqlModel.setCurrentIndex(currentIndex)
            }

        }
    }
    ColumnLayout{
        Layout.fillWidth: true
        implicitWidth: 1024
        //property var currentIndex
        id:tournamentView
        ListView{
            focus:true
            implicitHeight: 20;
            implicitWidth: 200;
            id:roundsView
            orientation: ListView.Horizontal
            //visible: model.rootIndex.valid
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            onCurrentIndexChanged: {
                matchesView.model.rootIndex = matchesModel.mapFromSource(roundsModel.createChildIndex(currentIndex,roundsView.model.rootIndex.row));
            }

            /*model:DelegateModel{
                model:roundsModel
                onRootIndexChanged: {
                    matchesView.model.rootIndex = matchesModel.mapFromSource(roundsModel.createChildIndex(currentIndex,roundsView.model.rootIndex.row));
                }

                delegate:Item{
                    width: 15;height: 15
                    Column{
                        Text{
                            text: number
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            roundsView.currentIndex = index
                        }
                    }

                }

            }*/
        }

        ListView{
            implicitHeight: 512;implicitWidth: 400
            focus:true
            id:matchesView
            //visible:model.rootIndex.valid

            /*model:DelegateModel{
                rootIndex: matchesModel.getRoot()
                model:matchesModel
                delegate:Item{
                    width: 200;height:25
                    Row{
                        Text{text:whiteRole}
                        Text{text:blackRole}
                    }
                }
            }*/
        }

        /*TreeView{
            id: matchesView
            model: matchesModel
            visible:rootIndex.valid
            rootIndex:
            selection: ItemSelectionModel{
                model:matchesModel
                id: matchesSelection
            }

            TableViewColumn{
                title:"White"
                role:"whiteRole"
            }

            TableViewColumn{
                title:"Black"
                role:"blackRole"
            }
        }*/
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
