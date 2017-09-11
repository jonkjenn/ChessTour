import QtQuick 2.0

                ColumnLabel{
                    text: if(enginemate==""){
                              var change = Math.round((enginescore-previousenginescore)/10)/10;
                              if(change>0){
                                  return "+" + change;
                              }
                              return change;
                          }
                          else{return "#";}
                    font.pixelSize: 12
                    color: if(enginemate!=""){
                               return "white"
                           }else if(enginescore-previousenginescore>0){
                               return "#adf442"
                           }else if(enginescore-previousenginescore<0){
                               return "#ff3923"
                           }else{
                               return "#70d6ff"
                           }
                }
