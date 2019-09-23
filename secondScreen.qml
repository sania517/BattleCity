import QtQuick 2.0


    Item {
        id:second_screen
        focus: staсkView.currentItem === this ? true : false
        Keys.onPressed: {
                if (event.key === Qt.Key_Left){
//                    console.error("press Left")
                    controllerMain.setDirectMoveTankOfPlayer(2);
                }
                if (event.key === Qt.Key_Right){
//                    console.error("press right")
                    controllerMain.setDirectMoveTankOfPlayer(0);
                }
                if (event.key === Qt.Key_Up){
//                    console.error("press Up")
                    controllerMain.setDirectMoveTankOfPlayer(1);
                }
                if (event.key === Qt.Key_Down){
//                    console.error("press Down")
                    controllerMain.setDirectMoveTankOfPlayer(3);
                }
                if (event.key === Qt.Key_Space){
//                    console.error("press Space")
                    controllerMain.setShotOfPlayer();
                }
//------------------------------------------------------------------------------------------
                if (event.key === Qt.Key_A){
//                    console.error("press Left 2nd A")
                    controllerMain.setDirectMoveTankOfPlayer2(2);
                }
                if (event.key === Qt.Key_D){
//                    console.error("press right 2nd D")
                    controllerMain.setDirectMoveTankOfPlayer2(0);
                }
                if (event.key === Qt.Key_W){
//                    console.error("press Up 2nd W")
                    controllerMain.setDirectMoveTankOfPlayer2(1);
                }
                if (event.key === Qt.Key_S){
//                    console.error("press Down 2nd S")
                    controllerMain.setDirectMoveTankOfPlayer2(3);
                }
                if (event.key === Qt.Key_Q){
//                    console.error("press Space 2nd Q")
                    controllerMain.setShotOfPlayer2();
                }
        }

        Connections{
            target: biCiFiModel
            onEndGame: {
                staсkView.push({item:Qt.resolvedUrl("ThirdScreen.qml")})
            }
        }



    GridView{
        id:grid_view_field
        x:14
        y:14
        height: 702
        width: 702
        cellWidth: 27
        cellHeight: 27
        anchors.margins: 0
        model:  qmlBiCiFi
        delegate: Item {
            height:grid_view_field.cellHeight
            width: grid_view_field.cellWidth
            Image {
                height: grid_view_field.cellHeight
                width: grid_view_field.cellWidth
                   id: field
                   source: model.urlField
            }
        }
    }

    GridView{
        id:grid_view_numberTank
        anchors.left: grid_view_field.right
        anchors.top: grid_view_field.top
        anchors.leftMargin: 20
        anchors.topMargin: 30
        height: 270
        width: 54
        cellWidth: 27
        cellHeight: 27
        model:  numberOfTank
        delegate: Item {
            height:grid_view_numberTank.cellHeight
            width: grid_view_numberTank.cellWidth
            Image {
                height: 21  //grid_view_numberTank.cellHeight*0.7
                width: 21 //grid_view_numberTank.cellWidth*0.7
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                   source: model.numberOfTanks
            }
        }
    }

    Rectangle{
        id :firstLifeScreen
        width: 64
        height: 60
        anchors.top: grid_view_numberTank.bottom
        anchors.left: grid_view_numberTank.left
        Image {
            source: "image/life 1pl.png"
            anchors.fill: parent
        }
        Text {
            font.pixelSize: 30
            text: biCiFiModel.lifeCount > -1 ? biCiFiModel.lifeCount : 0
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 10
        }
    }

    Rectangle{
        id : secondLifeScreen
        width: 64
        height: 60
        anchors.top: firstLifeScreen.bottom
        anchors.horizontalCenter:  firstLifeScreen.horizontalCenter
        anchors.topMargin: 20
        visible: biCiFiModel.second_Player ? true : false
        Image {
            source: "image/life 2pl.png"
            anchors.fill: parent
        }
        Text {
            font.pixelSize: 30
            text: biCiFiModel.lifeCount2 > -1 ? biCiFiModel.lifeCount2 : 0
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 10
        }
    }

    Rectangle{
        id : stage
        color: "#6f6262"
        width: 64
        height: 80
        anchors.top: secondLifeScreen.bottom
        anchors.horizontalCenter:  firstLifeScreen.horizontalCenter
        anchors.topMargin: 30
        Image {
            source: "image/stage.png"
            width:  parent.width
            height: parent.height - 20
        }
        Text {
            font.pixelSize: 30
            text: biCiFiModel.stage
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 10
        }
    }

    Rectangle{
        id: screenTank
        height: 702
        width: 702
        color: "transparent"
        x:14
        y:14
//        anchors.leftMargin: 14
//        anchors.topMargin: 14

        Connections{
            target: biCiFiModel
            onCreateObject :{
//                newObject.x = newObject.x + 14//screenTank.anchors.leftMargin
//                newObject.y = newObject.y + 14//screenTank.anchors.topMargin
                screenTank.data.push(newObject)
            }
        }
    }
    Rectangle{
        id: screenFlashBullet
        height: 729
        width: 729
        color: "transparent"
        Connections{
            target: biCiFiModel
            onCreateBulletFlashSignal: {

                screenFlashBullet.data.push(newObject)
            }
        }
    }

//    GridView{
//        id:grid_view_object
//        x: 14
//        y: 14
//        height: 702
//        width: 702
//        cellWidth: 27
//        cellHeight: 27
//        anchors.margins: 0
//        model:  tankData
//        delegate: Item {
//            height: grid_view_object.cellHeight*2
//            width: grid_view_object.cellWidth*2
//                    Image {
//                    id: objec
//                    anchors.verticalCenter: parent.verticalCenter
//                    anchors.horizontalCenter: parent.horizontalCenter
//                    rotation: model.directTank
//                    source: model.urlTank
//                    }
//        }
//        move: Transition {
//            NumberAnimation { properties: "x,y"; duration: 400 }
//        }
//        moveDisplaced: Transition {
//            NumberAnimation { properties: "x"; duration: 400 }
//        }
//    }

//    GridView{
//        id:grid_view_bullet_0
//        height: 729
//        width: 729
//        cellWidth: 27
//        cellHeight: 27
//        anchors.margins: 0

//        model:  bullet_0_Data
//        delegate: Item {
//            height: grid_view_bullet_0.cellHeight
//            width: grid_view_bullet_0.cellWidth
//               Image {
//                    id: bullet_0
//                    rotation: 0
//                    source: model.urlBullet_0
//                    anchors.verticalCenter: parent.verticalCenter
//                    anchors.horizontalCenter: parent.horizontalCenter
//                }
//            }
//        moveDisplaced: Transition {
//            NumberAnimation { properties: "x"; duration: 200 }
//        }
//    }

//    GridView{
//        id:grid_view_bullet_1
//        height: 729
//        width: 729
//        cellWidth: 27
//        cellHeight: 27
//        anchors.margins: 0

//        model:  bullet_1_Data
//        delegate: Item {
//            height: grid_view_bullet_1.cellHeight
//            width: grid_view_bullet_1.cellWidth
//               Image {
//                    id: bullet_1
//                    rotation: 270
//                    source: model.urlBullet_1
//                    anchors.verticalCenter: parent.verticalCenter
//                    anchors.horizontalCenter: parent.horizontalCenter
//                }
//            }
//        move: Transition {
//            NumberAnimation { properties: "y"; duration: 200 }
//        }
//    }

//    GridView{
//        id:grid_view_bullet_2
//        height: 729
//        width: 729
//        cellWidth: 27
//        cellHeight: 27
//        anchors.margins: 0
//        model:  bullet_2_Data
//        delegate: Item {
//            height: grid_view_bullet_2.cellHeight
//            width: grid_view_bullet_2.cellWidth
//               Image {
//                    id: bullet_2
//                    rotation: 180
//                    source: model.urlBullet_2
//                    anchors.verticalCenter: parent.verticalCenter
//                    anchors.horizontalCenter: parent.horizontalCenter
//                }
//            }
//        move: Transition {
//            NumberAnimation { properties: "x"; duration: 200 }
//        }
//    }

//    GridView{
//        id:grid_view_bullet_3
//        height: 729
//        width: 729
//        cellWidth: 27
//        cellHeight: 27
//        anchors.margins: 0
//        model:  bullet_3_Data
//        delegate: Item {
//            height: grid_view_bullet_3.cellHeight
//            width: grid_view_bullet_3.cellWidth
//               Image {
//                    id: bullet_3
//                    rotation: 90
//                    source: model.urlBullet_3
//                    anchors.verticalCenter: parent.verticalCenter
//                    anchors.horizontalCenter: parent.horizontalCenter
//                }
//            }
//            move: Transition {
//                NumberAnimation { properties: "y"; duration: 200 }
//            }
//    }

//    GridView{
//        id:grid_view_Flash
//        height: 729
//        width: 729
//        cellWidth: 27
//        cellHeight: 27
//        anchors.margins: 0
//        model:  qmlBiCiFi
//        delegate: Item {
//               Image {
//                    id: flash
//                    source: model.urlFlash
//                    anchors.centerIn: parent.Center
//                }
//            }
//    }

    Image {
        visible: biCiFiModel.gameOver ? true : false
        source: "image/game over.png"
        width: parent.width*0.15
        height: parent.height*0.055

        anchors.horizontalCenter: grid_view_field.horizontalCenter
        anchors.verticalCenter: grid_view_field.verticalCenter

    }
}
