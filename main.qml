import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQml.Models 2.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Chess tournament viewer")

    ColumnLayout{
        ListView{
            id: tournametView
            model: tourModel
            width: 300
            height: 400
            delegate: Text{text:name}
        }
    }
}
