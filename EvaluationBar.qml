import QtQuick 2.0

Rectangle{
    radius: 5
    color: "black"
    rotation: 180

    Rectangle{
        radius: 5
        color: "white"
        width: parent.width
        height: if(enginemate !== ""){
                    return enginemate>0?parent.height:0;
                }else if(enginescore === ""){
                    return parent.height/2;
                }else{
                    return Math.min(parent.height/2 + (parent.height/10 * enginescore/100),parent.height);
                }
    }
    Rectangle{
        height: 1
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        color:"red"
    }
}
