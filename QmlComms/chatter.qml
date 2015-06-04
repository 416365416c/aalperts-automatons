import QtQuick 1.1
import "QmlComms" 1.0

Rectangle {
    id: root
    property string me: ""
    Networking {
        id: network
        forwarding: true
        function beServer() {
            network.targetIP = "server"
            if (me == "") me = "server"
        }
        function beClient() {
            network.targetIP = ipIn.text
            if (me == "") me = "client"
        }
        function sendMsg(msg) {
            msg = me + ":" + msg + "\n"
            chatbox.text = chatbox.text + msg
            sendMessage(msg);
        }
        onMessageReceived: {
            chatbox.text = chatbox.text + message
        }
    }

    color: "white"
    width: 480
    height: 360
    Row {
        x: 8; y: 8
        spacing: 8
        Text {
            text: "Target IP"
        }
        TextInput {
            id: ipIn
            width: 360
            activeFocusOnPress: true
            text: "127.0.0.1"
            Rectangle {
                z: -1
                border.color: ipIn.activeFocus ? "grey" : "black"
                anchors.fill: parent
            }
        }
    }

    Row {
        x: 8; y: 32
        spacing: 8
        Text {
            text: "My Name"
        }
        TextInput {
            id: meIn
            width: 360
            activeFocusOnPress: true
            text: ""
            onAccepted: root.me = meIn.text
            Rectangle {
                z: -1
                border.color: meIn.activeFocus ? "grey" : "black"
                anchors.fill: parent
            }
        }
    }

    Row {
        x: 8
        y: 66
        spacing: 8
        Rectangle {
            color: "#CCCCCC"
            width: 82
            height: 32
            Text {
                anchors.centerIn: parent
                text: "Be Server"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: network.beServer();
            }
        }
        Rectangle {
            color: "#CCCCCC"
            width: 82
            height: 32
            Text {
                anchors.centerIn: parent
                text: "Be Client"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: network.beClient();
            }
        }
        Rectangle {
            color: network.server ? "green" : "red"
            width: 82
            height: 32
            Text {
                anchors.centerIn: parent
                text: "Is Server"
            }
        }
        Rectangle {
            color: network.valid ? "green" : "red"
            width: 82
            height: 32
            Text {
                anchors.centerIn: parent
                text: "Is Valid"
            }
        }
    }

    Text {
        id: chatbox
        x: 8
        y: 100
        height: 212
        width: parent.width - 16
        text: "Text will go below.\n"
    }

    TextInput 
    {
        id: textIn
        y: 320
        height: 32
        x: 8
        width: parent.width - 16
        activeFocusOnPress: true
        onAccepted: {network.sendMsg(textIn.text); textIn.text = '';}
        Rectangle {
            z: -1
            border.color: textIn.activeFocus ? "grey" : "black"
            anchors.fill: parent
        }
    }
}
