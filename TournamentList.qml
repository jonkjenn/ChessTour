import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Templates 2.2
import QtQuick.Controls.Material 2.2

TournamentListView{
    id: tournamentsList
    boundsBehavior: Flickable.StopAtBounds
    clip: true
    focus:true
    model: tournamentsSqlModel
    property int currentPK: -1
    spacing: 2

    delegate:ItemDelegate{
        id:control
        height: 35
        width: parent.width
        text:name
        highlighted: index === tournamentsList.currentIndex

        contentItem: Text{
            anchors.verticalCenter: control.verticalCenter
            anchors.left: control.left
            text: title==""?name:title
            color: control.Material.foreground
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
        }
        background: Rectangle{
            width:  control.width
            height: control.height
            color: control.highlighted?Material.color(Material.Amber):"transparent"
        }

        onClicked: {
            tournamentsList.currentIndex = index
            tournamentNameHeader.text = (title==""?name:title);
        }
    }

    onCurrentIndexChanged: {
        tournamentsSqlModel.setCurrentIndex(currentIndex)
    }
}
