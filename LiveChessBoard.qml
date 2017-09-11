import QtQuick 2.0

ChessBoard{
    property var pos: gameposition
    property string lightColor: "#965c4c"
    property string darkColor: "#eda490"


    onPosChanged: {
        if(pos == ""){console.log("EMPTY POS");return;}

        //TODO: remove this by removing only the changed pieces
        //This clears the board
        for(var i=0;i<64;++i){
            chessBoard.children[0].children[i].children[0].source = "";
            chessBoard.children[0].children[i].color = squareColor(i);
        }

        //Color the previous move
        if(lastmoves !== ""){
            chessBoard.children[0].children[moveToBoardIndex(lastmoves[3],0)].color = "#abbae8";
            chessBoard.children[0].children[moveToBoardIndex(lastmoves[3],2)].color = "#7489c9";
        }


        //Put pieces in positions
        for(var i=0;i<pos.length;i+=3){
            chessBoard.children[0].children[pos[i+1]+(7-pos[i+2])*8].children[0].source ="qrc:///Resources/"+ pos[i] + ".png";
        }
    }

    Component.onCompleted: {
        //Color the previous move
        if(lastmoves !== ""){
            chessBoard.children[0].children[moveToBoardIndex(lastmoves[3],0)].color = "#abbae8";
            chessBoard.children[0].children[moveToBoardIndex(lastmoves[3],2)].color = "#7489c9";
        }

        for(var i=0;i<pos.length;i+=3){
            chessBoard.children[0].children[pos[i+1]+(7-pos[i+2])*8].children[0].source ="qrc:///Resources/"+ pos[i] + ".png";
        }
    }

    //create index on the chessboard between 0-63 by converting the column letter to 0-7
    function moveToBoardIndex(move,start){
        return (move[start].charCodeAt()-97) + (8-move[start+1])*8;
    }

    //Generates the checker pattern
    function squareColor(index){
        if((9*Math.floor(index/8)-index-1)%2===0 ){
            return lightColor
        }else{
            return darkColor
        }
    }
}
