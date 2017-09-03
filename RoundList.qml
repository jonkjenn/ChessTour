import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

            ListView{
                focus:true
                implicitHeight: 50;
                id:roundsView
                clip:true
                orientation: ListView.Horizontal
                property int currentRoundPK: -1

                model:roundSqlModel

                delegate:ItemDelegate{
                    id:control
                    highlighted: index === roundsView.currentIndex
                    width: 35;height: 35;

                    contentItem:
                        Label{
                            text: number
                            font.pixelSize: 20
                            verticalAlignment: Label.AlignVCenter
                            horizontalAlignment: Label.AlignHCenter
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
                                          return Material.background;
                                      }else{
                                          console.log(startdate);
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
                    target: tournamentList
                    onCurrentPKChanged:{
                        console.log("Current tournament PK changed")
                        roundSqlModel.tournamentChanged(tournamentList.currentPK)
                        currentIndex = roundSqlModel.currentShownRound()
                        console.log("Current index " + currentIndex)
                        currentRoundPK = roundSqlModel.getPk(currentIndex)
                    }
                }

                onCurrentIndexChanged: {
                    currentRoundPK = roundSqlModel.getPk(currentIndex)
                    roundSqlModel.setCurrentIndex(currentIndex)
                }
            }
