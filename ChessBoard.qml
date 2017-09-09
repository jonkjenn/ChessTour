import QtQuick 2.0

Rectangle{
    id:outer
    property int squareSize: Math.floor(width/8)

Grid{
    anchors.fill:parent
    columns: 8
    rows:8
    Repeater{
        id:boardWrapper
        model:64
        delegate:
        Rectangle{
            width: outer.squareSize
            height: outer.squareSize
            color:if((9*Math.floor(index/8)-index-1)%2===0 ){
                      return "black"
                  }else{
                      return "white"
                  }
        Image{
            width: outer.squareSize
            height: outer.squareSize
        }

    }
    }

}
MouseArea{
    anchors.fill: outer
    onClicked:{
        var xindex = Math.floor(mouseX/outer.squareSize);
        var yindex = Math.floor(mouseY/outer.squareSize)*8;
        var i = board.itemAt(yindex+xindex);
        var j = i.children[0];
        j.source = "qrc:///Resources/qlt.png";
        console.log(outer.board.count);
    }
    }

}
