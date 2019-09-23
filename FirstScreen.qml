import QtQuick 2.0

Item{
id: firstScreen
focus: staсkView.currentItem === this ? true : false

    anchors.fill: parent

    Keys.onPressed: {
        if (event.key === Qt.Key_Up){
            if(listPointer.get(1).pointer){
                listPointer.move(1,0,1)
            }
            if(listPointer.get(2).pointer){
                listPointer.move(2,1,1)
            }
        }

        if (event.key === Qt.Key_Down){
            if(listPointer.get(1).pointer){
                listPointer.move(1,2,1)
            }
            if(listPointer.get(0).pointer){
                listPointer.move(0,1,1)
            }
        }
        if (event.key === Qt.Key_Space || event.key === Qt.Key_Enter){
            biCiFiModel.second_Player = listPointer.get(1).pointer
            console.error(biCiFiModel.second_Player)
            controllerMain.loadStage()
            staсkView.push({item:Qt.resolvedUrl("SecondScreen.qml")})
            controllerMain.startGame()



        }
    }

    Rectangle{
        anchors.fill: parent
        color: "transparent"
        Image {
            id: startScreen
            source: "image/start screen image.png"
            anchors.fill: parent
        }
    }


    ListView{
        anchors.top: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 0.35*staсkView.width
        anchors.topMargin: 0.0274*staсkView.height
        height: listButton.count*0.0685*staсkView.height
        model: listButton
        delegate: Rectangle{
            height: 0.0685*staсkView.height
            width: 0.35*staсkView.width
            color: mouseUse.pressed ? "grey" : "transparent" //цвет при нажатии мышкой назад
            Image {
                source: model.imageUrl
                height: parent.height/2.38
                width: sourceSize.width/290*parent.width
                anchors.verticalCenter:  parent.verticalCenter
                id: name
                MouseArea{
                    id: mouseUse
                    anchors.fill: parent
                    anchors.margins: -5
                    onClicked: {
                        if(index == 1)
                            biCiFiModel.second_Player = true
                        else
                            biCiFiModel.second_Player = false
                        console.error(biCiFiModel.second_Player)
                        controllerMain.loadStage()
                        staсkView.push({item:Qt.resolvedUrl("SecondScreen.qml")})
                        controllerMain.startGame()

                    }
                }
            }
        }
    }

    ListView{
        anchors.top: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 0.28*staсkView.width
        anchors.topMargin: 0.0274*staсkView.height
        width: 0.047*staсkView.width
        height:listPointer.count*0.0685*staсkView.height
        model: listPointer
        delegate: Rectangle{
            height: 0.0685*staсkView.height
            width: 0.047*staсkView.width
            color: "transparent"
                Image {
                source: model.pointer ? "image/pointer.png" : ""
                height: 0.054*staсkView.height
                width: 0.04756*staсkView.width
                anchors.verticalCenter:  parent.verticalCenter
            }
        }
    }
}

