import QtQuick 2.0
import QtQuick.Controls 2.2
import Game 1.0
import QtQuick.Dialogs 1.3

Rectangle {
    id: rectangle
    width: 1000
    height: 1000
    property bool gameOver: gameClient.gameOver
    onGameOverChanged: {
        if(gameClient.gameOver) {
            if(gameClient.youWin) {
                winDialog.open();
            } else {
                loseDialog.open();
            }


        }
    }
    Timer {
        id: gameOverTimer
        interval: 100 // 100 ms задержка
        running: false
        repeat: false
        onTriggered: {
            stackView.pop();
        }
    }


    ListView {
        id: enemyView
        width: parent.width
        height: parent.height /3
        anchors.top: parent.top
        anchors.topMargin: 0
        model: qmlGame.getEnemy()
        spacing: 5
        orientation: ListView.Horizontal

        delegate: EnemyItem {
            count: model.cardCount
            name: "user"
        }
    }

    ListView {
        id: tableViwe
        x: 0
        orientation: ListView.Horizontal
        width: parent.width
        anchors.bottom: handView.top
        anchors.bottomMargin: 0
        anchors.top: enemyView.bottom
        anchors.topMargin: 0
        model: qmlGame.getTable()
        delegate: CardItem {
            colorType: model.color
            cost: model.cost
        }
    }

    ListView {
        id: handView
        anchors.topMargin: 630
        orientation: ListView.Horizontal
        width: parent.width
        height: parent.height /3
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        model: qmlGame.getHand()
        delegate: CardItem {
            colorType: model.color
            cost: model.cost
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(gameClient.isYourMove()) {
                        gameClient.move(model.color, model.cost);
                    }
                }
            }
        }
    }
    Button {
        x: 439
        y: 929
        text: "Get card"
        onClicked: gameClient.getCard()
    }

    // Диалог для победы
    MessageDialog {
        id: winDialog
        title: "Результат"
        text: "Ты победил!"
        icon: StandardIcon.Information
        onAccepted: {
            gameOverTimer.start();
            console.log("Пользователь подтвердил победу")
        }
    }

    // Диалог для проигрыша
    MessageDialog {
        id: loseDialog
        title: "Результат"
        text: "Ты проиграл!"
        icon: StandardIcon.Critical
        onAccepted: {
            gameOverTimer.start();
            console.log("Пользователь подтвердил проигрыш")
        }
    }
}
