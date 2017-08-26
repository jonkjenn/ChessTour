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
            implicitWidth: 150
        Text{ id: tournamentHeader;text: "TournamentView"}
        TreeView{
            id: tournamentsList
            model: tournamentViewModel
            Layout.fillHeight: true
            selectionMode: SelectionMode.SingleSelection

            //onClicked: tournamentViewModel.sourceModel.setData(tournamentViewModel.mapToSource(currentIndex),

            onCurrentIndexChanged:{
                tournamentViewModel.currentTournament = tournamentViewModel.mapToSource(currentIndex).row
                roundsView.model.rootIndex = roundsModel.mapFromSource(tournamentViewModel.mapToSource(tournamentsList.currentIndex))
                //roundsSelection.setCurrentIndex(roundsModel.index(0,0,roundsView.rootIndex),0x0012)
                //console.log("Should have updated rounds currentIndex")
                //console.log(roundsView.currentIndex.row)
                //console.log(roundsSelection.currentIndex.row)
            }

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
        //property var currentIndex
        id:tournamentView
        ListView{
            focus:true
            implicitHeight: 20;
            implicitWidth: 200;
            id:roundsView
            orientation: ListView.Horizontal
            visible: model.rootIndex.valid
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            onCurrentIndexChanged: {
                    roundsModel.indexFromRow(0,roundsView.model.rootIndex)
                    //matchesView.model.rootIndex = matchesModel.mapFromSource(roundsModel.mapToSource(roundsModel.indexFromRow(roundsView.currentIndex,roundsView.model.rootIndex)))
                    console.log("Set root index")
                console.log(matchesView.model.rootIndex.row)
            }

            model:DelegateModel{
                model:roundsModel
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

            }
                    onCountChanged: {
                        console.log("Row: " + roundsView.model.rootIndex.row)
                        console.log("count: " + roundsView.count)
                    }
        }

        ListView{
            implicitHeight: 512;implicitWidth: 400
            focus:true
            id:matchesView
            visible:model.rootIndex.valid

            model:DelegateModel{
                model:matchesModel
                delegate:Item{
                    width: 200;height:25
                    Row{
                        Text{text:whiteRole}
                        Text{text:blackRole}
                    }
                }
            }
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
