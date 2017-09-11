import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtMultimedia 5.9
import QtQuick.Layouts 1.3

GridView{
    focus:true
    clip:true
    model:matchSqlModel
    cellHeight: boardSize+70
    cellWidth: boardSize + boardSize/12 + 60
    property int columns: 1
    property int boardSize: boardSizeSlider.value
    //visible:model.rootIndex.valid

    onWidthChanged: {

        columns = Math.max(1,Math.floor(width/cellWidth));
    }

    /*header: Item{
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
        }
    }*/

    delegate:ItemDelegate{
        background: Rectangle{
            id:matchRowBack
            implicitWidth: cellWidth - 8
            implicitHeight: cellHeight - 4
            //color:Math.floor(index/matchesView.columns)%2==0?"#3E3E3E":"#2E2E2E" //alternating rows
            color: resultwhite === ""?"#3e3e3e":"#2e2e2e"
            border.color: "transparent"
            border.width: 2
        }

        ColumnLayout{
            id:matchDataRow
            anchors.top:parent.top
            anchors.left: parent.left
            anchors.margins: 5
            //anchors.verticalCenter: matchRowBack.verticalCenter
            spacing: 10
            RowLayout{
            NameColumnLabel{
                text:blackfide
                color: resultblack>0.5?Material.color(Material.Amber):Material.foreground
                Layout.maximumWidth: boardSize
            }
            ColumnLabel{
                text:resultblack
                Layout.preferredWidth: 25
            }
            }
            RowLayout{

                LiveChessBoard{
                    id:chessBoard
                    Layout.preferredWidth:boardSize
                    Layout.preferredHeight:boardSize
                }

                EvaluationBar{
                    id:evalBar
                    Layout.preferredHeight: boardSize - 20
                    Layout.preferredWidth: height/12
                    anchors.top: chessBoard.top
                }

                    EvaluationChange{
                        anchors.left: evalBar.right
                        anchors.verticalCenter: evalBar.verticalCenter
                        anchors.leftMargin: 5
                        id:engineDiffLabel
                    }

                Evaluation{
                    id: engineLabel
                    clip:false
                    anchors.left:chessBoard.right
                    anchors.bottom:chessBoard.bottom
                    anchors.leftMargin: 5

                }

            }

            RowLayout{
            NameColumnLabel{
                text:whitefide
                color: resultwhite>0.5?Material.color(Material.Amber):Material.foreground
                Layout.maximumWidth: boardSize
            }
            ColumnLabel{
                text:resultwhite
                Layout.preferredWidth: 25
            }
            }
            /*Item{
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
            }*/

            ColumnLabel{
                text:status
                Layout.preferredWidth: 70
                visible: false
            }
        }

    }
}
