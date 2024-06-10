import QtQuick 2.0
import QtQuick.Controls 2.2
import Game 1.0
Rectangle {
    Column {
        Button {
            text: "Back"
            onClicked: {
                stackView.pop();
            }
        }

        Text {
            text: "Leaderboard"
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 20
        }

        ListView {
            height: 300
            width: 300
            model: qmlGame.getLeaderboard()
            delegate: Rectangle {

                width: parent.width
                height: 20
                Row {
                    Text {
                        width: 100
                        text: model.login

                    }
                    Text {
                        width: 50
                        text: model.record
                    }
                }
            }
        }
    }
}
