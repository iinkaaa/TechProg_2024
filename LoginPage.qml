import QtQuick 2.0
import QtQuick.Controls 2.2
Rectangle {
    width: 1000
    height: 100
    property bool isLogin: gameClient.login
    onIsLoginChanged: {
        stackView.push("qrc:/GameLobby.qml")
    }
    Column {
        spacing: 10
        anchors.centerIn: parent
        TextField {
            id: usernameField
            placeholderText: "Username"
        }
        TextField {
            id: passwordField
            placeholderText: "Password"
            echoMode: TextInput.Password
        }
        Button {
            text: "Login"
            onClicked: {
                gameClient.tryToLogin(usernameField.text, passwordField.text);

            }
        }
        Button {
            text: "Registr"
            onClicked: {
                stackView.push("qrc:/RegistrPage.qml")
            }
        }
    }
}
