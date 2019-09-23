import QtQuick 2.0
import QtGraphicalEffects 1.0

Image {
    id: imageOfObject
//    source: ""
    property int imageX:0
    property int imageY:0
    property int speed :0
    property int speedRotation: 0
    property bool bonus: false
    property bool helm: false
    x:imageX //+ parent.x
    y:imageY //+ parent.y
    rotation: 0

    Connections{
        target: biCiFiModel
    }

    Behavior on x { NumberAnimation { duration: speed * 200 }}
    Behavior on y { NumberAnimation { duration: speed * 200 }}
    Behavior on rotation { NumberAnimation { duration: speedRotation * 200 }}

    Timer {
        id: timer
        interval: 750
        repeat: true
        running: false
        onTriggered: {
            if (bonus){
                if (colorOverlay.color == "#00000000"){
                    colorOverlay.color = "#80ff0000"
                }
                else{
                    colorOverlay.color = "#00000000"
                }
            } else if (helm){
                if (colorOverlay.color == "#00000000"){
                    colorOverlay.color = "#800000ff"
                }
                else{
                    colorOverlay.color = "#00000000"
                }
            }
        }
    }
    onBonusChanged: {
        console.error("qml bonus: ", bonus)
        if (bonus)
            timer.start()
        else{
            timer.stop()
            colorOverlay.color = "#00000000"
        }
    }
    onHelmChanged: {

        console.error("qml helm: ", helm)
        if (helm)
            timer.start()
        else{
            timer.stop()
            colorOverlay.color = "#00000000"
        }
    }



    ColorOverlay {
        id : colorOverlay
            anchors.fill: imageOfObject
            source: imageOfObject
            color: "#00000000"
        }


}
