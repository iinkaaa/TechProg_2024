import QtQuick 2.0
import QtQuick.Controls 2.2
import Game 1.0
//import "CardItem.qml"
Rectangle {
    width: 300
    height: 300

    Column {
        spacing: 10
        anchors.centerIn: parent
        Button {
            text: "New game"
            onClicked: {
                gameClient.startGame();
            }
        }
        Button {
            text: "Ready/Continue"
            onClicked: {
                gameClient.ready();
                stackView.push("qrc:/Game.qml")
            }
        }
        Button {
            text: "Leaderboard"
            onClicked: {
                gameClient.leaderboard()
                stackView.push("qrc:/Leaderboard.qml")
            }
        }

    }

}
