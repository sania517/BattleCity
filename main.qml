import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4


ApplicationWindow {
    id: mainWindow
    visible: true
    width: 830
    height: 730
    title: qsTr("Battle City")

    property int  lightTank1p: 0
    property int  lightTank2p: 0
    property int  speedTank1p: 0
    property int  speedTank2p: 0
    property int  heavyTank1p: 0
    property int  heavyTank2p: 0
    property int  heavyDutyTank1p: 0
    property int  heavyDutyTank2p: 0

    Connections{
        target: biCiFiModel
        onEndGame: {
            lightTank1p = lightTank_1p
            lightTank2p = lightTank_2p
            speedTank1p = speedTank_1p
            speedTank2p = speedTank_2p
            heavyTank1p = heavyTank_1p
            heavyTank2p = heavyTank_2p
            heavyDutyTank1p = durtyheavyTank_1p
            heavyDutyTank2p = durtyheavyTank_2p
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem{
                text: qsTr("&Open old game")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem{
                text: qsTr("&Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    ListModel{
        id: listButton

        ListElement{
            nameButton: "1 player button"
            imageUrl: "image/1 player button.png"
        }
        ListElement{
            nameButton: "2 player button"
            imageUrl: "image/2 player button.png"
        }
        ListElement{
            nameButton: "construction button"
            imageUrl: "image/construction button.png"
        }
    }

    ListModel{
        id: listPointer
        ListElement{
            pointer: true
        }
        ListElement{
            pointer: false
        }
        ListElement{
            pointer: false
        }
    }

    StackView{
        id: staсkView
        width: parent.width
        height: parent.height
        focus: true
        initialItem: FirstScreen{}
    }
}
