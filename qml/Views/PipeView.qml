import QtQuick 2.0

Item {
    Image {
        id: image
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        source: "qrc:/content/images/bg/pipe.svg"

        property var ledX: [304, 336, 285, 431, 615, 916, 323, 670, 965]
        property var ledY: [13, 111, 293, 67, 73, 65, 254, 269, 266]
        property var ledChan: [devDOStatus.relayChannel, devDOStatus.pumpChannel]
        property var arrIndx: [0, 0, 0, 0, 0, 0, 1, 1, 1]
        property var arrOffset: [0, 1, 2, 3, 4, 5, 0, 1, 2]

        Repeater {
            model: 9

            Loader {
                sourceComponent: led
                x: (image.ledX[index] - 6)*image.width/1004
                y: (image.ledY[index] - 6)*image.height/303
                property bool status: device.doStatus[image.ledChan[image.arrIndx[index]][image.arrOffset[index]]]
            }
        }
    }

    Component {
        id: led

        Rectangle {
            width: 12
            height: width
            color: "black"

            Rectangle {
                anchors.centerIn: parent
                width: 10
                height: width
                color: status == true ? "green" : "red"
            }
        }
    }
}
