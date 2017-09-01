import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

ListView{
    focus:true
    id:matchesView
    clip:true
    model:matchSqlModel
    //visible:model.rootIndex.valid

    header: Item{
        implicitWidth: 600;
        implicitHeight: 35
        Row{
            anchors.centerIn: parent
            spacing:10
            ColumnLabel{
                text:"White"
                width: 150
            }
            ColumnLabel{
                text:"Black"
                width: 150
            }
            ColumnLabel{
                text:"Result"
                width: 70
            }
            ColumnLabel{
                text:"Engine"
                width: 50
            }
            ColumnLabel{
                text:"Status"
                width: 70
            }

        }
    }

    delegate:ItemDelegate{
        background: Rectangle{
            implicitWidth: 600;
            implicitHeight: 35
            color:index%2==0?"#3E3E3E":"#2E2E2E"
        }

        Row{
            anchors.centerIn: parent
            spacing: 10
            NameColumnLabel{
                text:whitefide
                width: 150
            }
            NameColumnLabel{
                text:blackfide
                width: 150
            }
            ColumnLabel{
                text:resultwhite
                width: 35
            }
            ColumnLabel{
                text:resultblack
                width: 35
            }
            ColumnLabel{
                id: engineLabel
                text:enginemate==""?enginescore/100.0:enginemate
                color: if(enginemate != ""){
                           return "yellow"
                       }else{
                           if(enginescore>0){
                               return "green"
                           }else if(enginescore == 0){
                               return "blue"
                           }else{
                               return "red"
                           }
                       }
                background: Rectangle{
                    id: engineBack
                    opacity: 0
                    color: "#BEBEBE"
                }

                onTextChanged:{
                    console.log("Text changed " + engineLabel.text)
                    updateHighlight.start()
                }
                SequentialAnimation{
                    id: updateHighlight

                    NumberAnimation {
                        target: engineBack
                        property: "opacity"
                        duration: 750
                        easing.type: Easing.InOutQuad
                        to: 1
                    }
                    PauseAnimation{
                        duration: 1000
                    }
                    NumberAnimation {
                        target: engineBack
                        property: "opacity"
                        duration: 750
                        easing.type: Easing.InOutQuad
                        to: 0
                    }
                }


                width: 50
            }
            ColumnLabel{
                text:status
                width: 70
            }
        }

    }

    Connections{
        target:roundsView
        onCurrentRoundPKChanged:{
            matchSqlModel.setRound(roundsView.currentRoundPK)
        }
    }
}
