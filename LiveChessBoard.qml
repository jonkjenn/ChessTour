import QtQuick 2.0

                ChessBoard{
                    property var pos: gameposition

                    onPosChanged: {

                        for(var i=0;i<64;++i){
                            chessBoard.children[0].children[i].children[0].source = "";
                        }

                        for(var i=0;i<pos.length;i+=3){
                            chessBoard.children[0].children[pos[i+1]+(7-pos[i+2])*8].children[0].source ="qrc:///Resources/"+ pos[i] + ".png";
                        }
                    }

                    Component.onCompleted: {

                        for(var i=0;i<pos.length;i+=3){
                            chessBoard.children[0].children[pos[i+1]+(7-pos[i+2])*8].children[0].source ="qrc:///Resources/"+ pos[i] + ".png";
                        }
                    }
                }
