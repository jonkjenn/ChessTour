import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtMultimedia 5.9
import QtQuick.Layouts 1.3

GridView{
    focus:true
    id:matchesView
    clip:true
    model:matchSqlModel
    cellHeight: boardSize+4
    cellWidth: 750
    property int columns: 1
    property int boardSize: 128
    //visible:model.rootIndex.valid

    onWidthChanged: {

        columns = Math.max(1,Math.floor(width/cellWidth));
        console.log("COLUMNS: " + columns);
    }

    header: Item{
        implicitWidth: 550;
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
            implicitWidth: 548;
            implicitHeight: chessBoard.height
            color:Math.floor(index/matchesView.columns)%2==0?"#3E3E3E":"#2E2E2E"
        }

        RowLayout{
            id:matchDataRow
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: matchRowBack.verticalCenter
            spacing: 10
            NameColumnLabel{
                text:whitefide
                color: resultwhite>0.5?Material.color(Material.Amber):Material.foreground
                Layout.preferredWidth: 150
            }
            NameColumnLabel{
                text:blackfide
                color: resultblack>0.5?Material.color(Material.Amber):Material.foreground
                Layout.preferredWidth: 150
            }
            ColumnLabel{
                text:resultwhite
                Layout.preferredWidth: 35
            }
            ColumnLabel{
                text:resultblack
                Layout.preferredWidth: 35
            }
            Item{
                Layout.preferredWidth: 50
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


                Layout.preferredWidth: 50
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
                Layout.preferredWidth: 70
            }
            /*Text{
                id:gamePosition
                visible: false
                width: 50
                text: gameposition

                onTextChanged: {
                    var pos = JSON.parse(gameposition);
                }
            }*/

            ChessBoard{
                id:chessBoard
                Layout.preferredWidth:boardSize
                Layout.preferredHeight:boardSize

                Component.onCompleted:{
                    var pos = JSON.parse(gameposition);

                    for(var i=0;i<pos.length;i+=3){
                        /*console.log(pos[i+1]+pos[i+2]*7);
                        console.log("x: " + pos[i+1] + " y:" + (pos[i+2]*7));
                        console.log(chessBoard.children[0]);
                        console.log(chessBoard.children[0].rows);
                        console.log(chessBoard.children[0].columns);
                        console.log(chessBoard.children[0].children[0]);
                       console.log(chessBoard.children[0].children[pos[i+1]+pos[i+2]*7].children[0]);*/

                        chessBoard.children[0].children[pos[i+1]+pos[i+2]*8].children[0].source ="qrc:///Resources/"+ pos[i] + ".png";


                        //chessBoard.board.itemAt(pos[i+1]+pos[i+2]*7).children[0].source = "qrc:///Resources/"+ pos[i] + ".png";
                    }
                }
            }

            /*ColumnLabel{
                text:previousenginemate==""?previousenginescore/100.0:"#"+previousenginemate
                width: 50
            }*/
        }

    }
}
