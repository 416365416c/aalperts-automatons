import QtQuick 2.0;

Item {
    id: window
    width: 320;
    height: 480;

    Rectangle {
        width: 320;
        height: 480;
        color: "#1da6ff";

        Text {
            id: text1
            x: 20
            y: 40
            width: 280
            height: 200
            text: qsTr("Welcome to Bragi")
            font.bold: true
            wrapMode: Text.WordWrap
            font.pixelSize: 30
            color: "white"
        }

        Text {
            id: text2
            x: 20
            y: 75
            width: 280
            height: 200
            text: qsTr("Realtime design editor for QML")
            font.bold: true
            wrapMode: Text.WordWrap
            font.pixelSize: 18
            color: "white"
        }

        Text {
            id: text3
            x: 20
            y: 200
            width: 280
            height: 200
            text: qsTr("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.")
            font.pixelSize: 12
            wrapMode: Text.WordWrap
            color: "yellow"
        }
    }

    // The shadow for the ball
    Image {
        anchors.horizontalCenter: parent.horizontalCenter
        y: ball.minHeight + 90
        source: "snippets/images/welcome/ball-shadow.png"

        // The scale property depends on the y position of the ball face.
        scale: ball.y * 0.5 / (ball.minHeight - ball.maxHeight)
    }

    Image {
        id: ball
        property int maxHeight: window.height / 3
        property int minHeight: 2 * window.height / 3

        anchors.horizontalCenter: parent.horizontalCenter
        y: minHeight
        source: "snippets/images/welcome/ball.png"

        // Animate the y property. Setting loops to Animation.Infinite makes the
        // animation repeat indefinitely, otherwise it would only run once.
        SequentialAnimation on y {
            loops: Animation.Infinite

            // Move from minHeight to maxHeight in 300ms, using the OutExpo easing function
            NumberAnimation {
                from: ball.minHeight; to: ball.maxHeight
                easing.type: Easing.OutExpo; duration: 300
            }

            // Then move back to minHeight in 1 second, using the OutBounce easing function
            NumberAnimation {
                from: ball.maxHeight; to: ball.minHeight
                easing.type: Easing.OutBounce; duration: 800
            }

            // Then pause for 500ms
            PauseAnimation { duration: 800 }
        }
    }

}
