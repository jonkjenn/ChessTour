import QtQuick 2.0
import QtQuick.Controls 2.2

                Button{
                            width: 32
                            height: 32
                        background: Image{
                            id:volumeButtonImage
                            source: volumeSlider.volume>0?"qrc:///speaker.png":"qrc:///speakeroff.png";
                            sourceSize.height: 32
                            sourceSize.width: 32
                        }
                        onClicked: if(volumeSlider.volume>0){
                                       volumeSlider.previousVolume = volumeSlider.value;
                                       volumeSlider.value=0;
                                   }else{
                                       volumeSlider.value = volumeSlider.previousVolume;
                                   }
                    }
