import QtQuick 2.0

Rectangle {
    id: root
    property int count
    property string name
    width: 100
    height: 100
    color: "gray"
    Column {
        anchors.fill: parent
        // Text {
        //     // anchors.fill: parent
        //     height: 100
        //     width: 100
        //     text: "Игрок " + name
        //     verticalAlignment: Text.AlignVCenter
        //     horizontalAlignment: Text.AlignHCenter
        //     color: "white"

        // }
        Text {
            height: 100
            width: 100
            text: "Карт " + count
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: "white"

        }

    }


}
