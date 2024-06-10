import QtQuick 2.0
import QtQuick.Controls 2.2
Rectangle {
    width: 300
    height: 300
    property bool isRegistr: gameClient.isRegistr
    onIsRegistrChanged: {
        stackView.pop()
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
        TextField {
            id: repeatPasswordField
            placeholderText: "Repeat password"
            echoMode: TextInput.Password
        }
        Button {
            text: "Registr"
            onClicked: {
                if(passwordField.text == repeatPasswordField.text) {
                    gameClient.registr(usernameField.text, passwordField.text);
                }
            }
        }
        Button {
            text: "back"
            onClicked: {
                stackView.pop();
            }
        }
    }
}
