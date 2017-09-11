import QtQuick 2.0
import QtQuick.Controls.Material 2.2

                ColumnLabel{
                    property int changes: 0
                    text:enginemate==""?Math.round(enginescore/10)/10.0:"#"+enginemate
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
                        onTriggered: matchRowBack.border.color = "transparent"
                    }

                    onTextChanged:{
                        if(changes>=1){
                            matchRowBack.border.color = Material.color(Material.Amber)
                            opacityTimer.restart();

                            if(Math.abs(enginescore-previousenginescore)/100.0>0.5){
                                playMusic.play();
                            }

                        }else{
                            changes = 1;
                        }
                    }

                }

