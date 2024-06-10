import QtQuick 2.9
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
Window
{
    width: 1000
    height: 1000
visible: true
StackView {
    id: stackView
    initialItem: "qrc:/LoginPage.qml"
    anchors.fill: parent
}
}
