import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

ListView{
    focus:true
    id:roundsView
    clip:true
    orientation: ListView.Horizontal
    property int currentRoundPK: -1

    model:roundSqlModel

    delegate:ItemDelegate{
        id:control
        highlighted: index === roundsView.currentIndex
        width: 35;height: 35

        contentItem:
            Label{
            text: number
            font.pixelSize: 20
            verticalAlignment: Label.AlignVCenter
            horizontalAlignment: Label.AlignHCenter
            anchors.verticalCenter: control.verticalCenter
        }

        background: Rectangle{
            width:  control.width
            height: control.height
            color: if(control.highlighted){
                       return Material.color(Material.Amber);
                   }else{
                       if(ma.containsMouse){
                           return Material.color(Material.Grey)
                       }
                       return "transparent"
                   }
            border.width: 2
            border.color: if(startdate == undefined || startdate == ""){
                              console.log("Start date: " + startdate)
                              return Material.background;
                          }else{
                              console.log("Start date: " + startdate)
                              var d = new Date(startdate);
                              if(d.toDateString() == new Date().toDateString()){
                                  return "#7af442";
                              }else{
                                  return Material.background;
                              }
                          }
        }

        MouseArea {
            id:ma
            anchors.fill: parent
            onClicked: {
                roundsView.currentIndex = index
            }
            hoverEnabled: true
        }

    }

    Connections{
        target: roundSqlModel
        onRoundChanged:{
            currentIndex = roundSqlModel.currentRoundIndex();
        }
    }

    onCurrentIndexChanged: {
        roundSqlModel.setCurrentIndex(currentIndex)
    }
}
