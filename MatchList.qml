import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtMultimedia 5.9

ListView{
    focus:true
    id:matchesView
    clip:true
    model:matchSqlModel
    //visible:model.rootIndex.valid
    Audio {
        id: playMusic
        source: "qrc:///ding.wav"
    }

    header: Item{
        implicitWidth: 600;
        implicitHeight: 35
        Row{
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
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
            /*ColumnLabel{
                text:"PreviousEngine"
                width: 70
            }*/

        }
    }

    delegate:ItemDelegate{
        background: Rectangle{
            id:matchRowBack
            implicitWidth: 600;
            implicitHeight: 35
            color:index%2==0?"#3E3E3E":"#2E2E2E"
        }

        Row{
            id:matchDataRow
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: matchRowBack.verticalCenter
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
            Item{
                width: 50
                height: parent.height

                Rectangle{
                    id:engineBorder
                    width: parent.width
                    height: parent.height
                    color: "transparent"
                    border.color: "transparent"
                    border.width: 2
                    radius:5
                }

            ColumnLabel{
                property int changes: 0
                id: engineLabel
                text:enginemate==""?enginescore/100.0:"#"+enginemate
                color: if(enginemate != ""){
                           return "yellow"
                       }else{
                           if(enginescore>0){
                               return "#adf442"
                           }else if(enginescore == 0){
                               return "#70d6ff"
                           }else{
                               return "#ff3923"
                           }
                       }

                Timer{
                    id:opacityTimer
                    interval: 4000
                    running: false
                    repeat: false
                    onTriggered: engineBorder.border.color = "transparent"
                }

                /*SequentialAnimation{
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
                }*/


                width: 50
                onTextChanged:{
                    if(changes>=1){
                        engineBorder.border.color = Material.color(Material.Amber)
                        opacityTimer.restart();

                      if(Math.abs(enginescore-previousenginescore)/100.0>0.5){
                          playMusic.play();
                      }

                    }else{
                        changes = 1;
                    }
                }

            }

                ColumnLabel{
                    anchors.bottom:engineLabel.top
                    anchors.right: parent.right
                    id:engineDiffLabel
                    text: if(enginemate==""){
                              var change = (enginescore-previousenginescore)/100;
                              if(change>0){
                                  return "+" + change;
                              }
                              return change;
                          }
                          else{return "mate";}
                    font.pixelSize: 12
                    color: if(enginemate!=""){
                               return "white"
                           }else if(enginescore-previousenginescore>0){
                               return "#adf442"
                           }else if(enginescore-previousenginescore<0){
                               return "#ff3923"
                           }else{
                               return "#70d6ff"
                           }
                }
            }
            ColumnLabel{
                text:status
                width: 70
            }
            /*ColumnLabel{
                text:previousenginemate==""?previousenginescore/100.0:"#"+previousenginemate
                width: 50
            }*/
        }

    }
}
