import QtQuick 2.0

Item{
    id:third_screen
    anchors.fill: parent

    focus: staсkView.currentItem === this ? true : false

    //signal resetModelSignal

    Connections{
        target:biCiFiModel
    }

    Keys.onPressed: {

            if (event.key === Qt.Key_Left){
                if (biCiFiModel.gameOver){
                    staсkView.pop()
                    staсkView.pop()
                } else {
                    biCiFiModel.resetModelSlot()
                    staсkView.pop()
                    staсkView.pop()
                    controllerMain.loadStage()
                    staсkView.push({item:Qt.resolvedUrl("SecondScreen.qml")})
                    controllerMain.startGame()

                }

                console.error("press Left")


            }
            if (event.key === Qt.Key_Right){
                console.error("press Right")
                Qt.quit()

            }
    }

    ListModel{
        id : pointFirstPlayer
        ListElement{
           numberKill1P : 0
           numberKill2P : 0
        }

    }

    Timer{
        id: timer
        interval: 1000
        repeat: true
        running: true
        onTriggered: {
            console.error("pointFirstPlayer.count: " +pointFirstPlayer.count)
            switch(pointFirstPlayer.count){

            case 1 :
                pointFirstPlayer.append({numberKill1P:mainWindow.speedTank1p, numberKill2P:mainWindow.speedTank2p})
                break
            case 2 :
                pointFirstPlayer.append({numberKill1P:mainWindow.heavyTank1p, numberKill2P:mainWindow.heavyTank2p})
                break
            case 3:
                pointFirstPlayer.append({numberKill1P:mainWindow.heavyDutyTank1p, numberKill2P:mainWindow.heavyDutyTank2p})
                break
            case 4:
                pointFirstPlayer.append({numberKill1P: (pointFirstPlayer.get(0).numberKill1P +
                                                        pointFirstPlayer.get(1).numberKill1P +
                                                        pointFirstPlayer.get(2).numberKill1P +
                                                        pointFirstPlayer.get(3).numberKill1P),
                                         numberKill2P:pointFirstPlayer.get(0).numberKill2P +
                                                      pointFirstPlayer.get(1).numberKill2P +
                                                      pointFirstPlayer.get(2).numberKill2P +
                                                      pointFirstPlayer.get(3).numberKill2P})
                break
            default:
                timer.stop()
            }
        }
    }


        Rectangle{
            anchors.fill: parent
            color: "grey"
            Image{
                anchors.fill: parent
                source: "image/Third.png"
            }
        }

        Rectangle{
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: 0.07 * parent.width
            height: 0.08 * parent.height
            color: ma2.pressed ? "grey" : "transparent" //цвет при нажатии мышкой назад

            MouseArea{
                id: ma2
                anchors.fill: parent
                onClicked: {
                    Qt.quit()

                }
            }

            Image {
                anchors.fill: parent
                id: next
                source: "image/navigation_next_item.png"
            }
        }

        Rectangle{
            width: 0.07 * parent.width
            height: 0.08 * parent.height
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            color: ma1.pressed ? "grey" : "transparent" //цвет при нажатии мышкой назад

            MouseArea{
                id: ma1
                anchors.fill: parent
                onClicked: {
                    if (biCiFiModel.gameOver){
                        staсkView.pop()
                        staсkView.pop()
                    } else {
                        biCiFiModel.resetModelSlot()
                        staсkView.pop()
                        staсkView.pop()
                        controllerMain.loadStage()
                        staсkView.push({item:Qt.resolvedUrl("SecondScreen.qml")})
                        controllerMain.startGame()
                    }
                }
            }

            Image {
                anchors.fill: parent

                id: previous
                source: "image/navigation_previous_item.png"
            }
        }

        Rectangle{
            width: parent.width
            height: 2
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0.08 * parent.height
            color: "grey"
        }

        ListView{
            id: numberKill1P
            height: 0.5*parent.height
            width: 0.06*parent.width

            anchors.top: parent.top
            anchors.topMargin: 0.38*parent.height
            anchors.left: parent.left
            anchors.leftMargin: 0.3675*parent.width
            model: pointFirstPlayer

            delegate: Rectangle{
                color: "transparent"
                height: 0.107*third_screen.height
                width: 0.06*third_screen.width
                Text{
                    color:"white"
                    font.pixelSize: 0.0479*third_screen.height
                    horizontalAlignment : Text.AlignRight
                   text : model.numberKill1P
                }
            }
//            add: Transition {
//                NumberAnimation { properties: "x,y";  duration: 1000 }
//            }
        }
        ListView{
            id: pointKill1P
            height: 0.427*third_screen.height
            width: 0.06*third_screen.width

            anchors.top: parent.top
            anchors.topMargin:  0.38*parent.height
            anchors.left: parent.left
            anchors.leftMargin: 0.12*parent.width
            model: pointFirstPlayer

            delegate: Rectangle{
                color: "transparent"
                height: 0.107*third_screen.height
                width: 0.12*third_screen.width
                Text{
                    color:"white"
                    font.pixelSize: 0.0479*third_screen.height
                    horizontalAlignment : Text.AlignRight
                   text : model.numberKill1P*100*(index+1)
                }
            }
//            add: Transition {
//                SequentialAnimation{
//                    NumberAnimation { properties: "x,y";  duration: 1000 }
//                }


//            }
        }
        ListView{
            id: numberKill2P
            visible: biCiFiModel.second_Player ? true : false
            height: 0.5*third_screen.height
            width: 0.06*third_screen.width

            anchors.top: parent.top
            anchors.topMargin: 0.38*parent.height
            anchors.left: parent.left
            anchors.leftMargin: 0.584*parent.width
            model: pointFirstPlayer

            delegate: Rectangle{
                color: "transparent"
                height: 0.107*third_screen.height
                width: 0.06*third_screen.width
                Text{
                    color:"white"
                    font.pixelSize: 0.0479*third_screen.height
                    horizontalAlignment : Text.AlignRight
                   text : model.numberKill2P
                }
            }
//            add: Transition {
//                NumberAnimation { properties: "x,y";  duration: 1000 }
//            }
        }
        ListView{
            id: pointKill2P
            height: 0.427*third_screen.height
            width: 0.12*third_screen.width
            visible: biCiFiModel.second_Player ? true : false

            anchors.top: parent.top
            anchors.topMargin: 0.38*parent.height
            anchors.left: parent.left
            anchors.leftMargin: 0.687*parent.width
            model: pointFirstPlayer

            delegate: Rectangle{
                color: "transparent"
                height: 0.107*third_screen.height
                width: 0.12*third_screen.width
                Text{
                    color:"white"
                    font.pixelSize: 0.0479*third_screen.height
                    horizontalAlignment : Text.AlignRight
                   text : model.numberKill2P*100*(index+1)
                }
            }
//            add: Transition {
//                NumberAnimation { properties: "x,y";  duration: 1000 }
//            }
}





Component.onCompleted: {
    pointFirstPlayer.set(0,{numberKill1P:mainWindow.lightTank1p, numberKill2P:mainWindow.lightTank2p})


//    pointFirstPlayer.append({numberKill1P:mainWindow.speedTank1p, numberKill2P:mainWindow.speedTank2p})
//    pointFirstPlayer.append({numberKill1P:mainWindow.heavyTank1p, numberKill2P:mainWindow.heavyTank2p})
//    pointFirstPlayer.append({numberKill1P:mainWindow.heavyDutyTank1p, numberKill2P:mainWindow.heavyDutyTank2p})
//    pointFirstPlayer.append({numberKill1P: (pointFirstPlayer.get(0).numberKill1P+ pointFirstPlayer.get(1).numberKill1P + pointFirstPlayer.get(2).numberKill1P +
//                                           pointFirstPlayer.get(3).numberKill1P),
//                            numberKill2P:pointFirstPlayer.get(0).numberKill2P+ pointFirstPlayer.get(1).numberKill2P + pointFirstPlayer.get(2).numberKill2P +
//                                         pointFirstPlayer.get(3).numberKill2P})
}


}
