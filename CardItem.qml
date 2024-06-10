import QtQuick 2.0

Rectangle {
    id: root
    property int colorType: 1
    property int cost: 10
    property color cardColor: getColor(colorType)
    width: 100
    height: 200
    radius: 20
    color: cardColor
    Text {
        id: text1
//        font.pointSize: 12
        text: root.cost
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        font.bold: true
        font.pixelSize: 20
        color: "white"
    }

    Text {
        id: text3
        x: 168
        y: 275
        font.bold: true
//        font.pointSize: 12
        text: root.cost
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 40
        color: "white"
        anchors.fill: parent
    }

    Text {
        id: text2
        x: 168
        y: 275
        font.bold: true
//        font.pointSize: 12
        text: root.cost
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        font.pixelSize: 20
        color: "white"
    }

    Rectangle {
        width: parent.width / 1.5
        height: parent.width / 1.5
        border.color: "white"
        border.width: 10
        radius: width
        color: "transparent"
        anchors.centerIn: parent
    }

    function getColor(index) {
        switch(index) {
        case 0: return "red";
        case 1: return "blue";
        case 2: return "yellow";
        case 3: return "green";
        }
    }

    onColorChanged: {
        switch(root.color) {
        case 0: root.cardColor = "red"; break;
        case 1: root.cardColor = "blue"; break;
        case 2: root.cardColor = "yellow"; break;
        case 3: root.cardColor = "green"; break;
        }
    }
}
