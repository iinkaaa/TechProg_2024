import QtQuick 2.9
import QtQuick.Window 2.1
import QtQuick.Controls 1.4
Window
{
    width: 200
    height: 200

StackView {
    id: stackView
    initialItem: "LoginPage.qml"
    anchors.fill: parent
}
}
