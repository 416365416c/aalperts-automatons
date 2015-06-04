import QtQuick 2.0
import Bragi 0.1 as Logic

Item {
    id: container

    property alias source: interpreter.source
    property alias itemWidth: interpreter.width
    property alias scale: interpreter.scale

    Logic.Interpreter { 
        id: interpreter 
        anchors.centerIn: parent
        Behavior on scale {
            NumberAnimation { duration: 150; easing.type: Easing.OutQuad }
        }
    }


    Item {
        id: errorBanner
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.top
        anchors.bottomMargin: interpreter.valid ? 0 : -height
        width: parent.width * 0.8
        height: errorText.height * 2

        Image {
            anchors.fill: parent
            source: "images/error-banner.png"
            fillMode: Image.TileHorizontally
            opacity: 0.8
        }

        Behavior on anchors.bottomMargin {
            NumberAnimation { duration: 150; easing.type: Easing.OutQuad }
        }

        Text {
            id: errorText
            y: 20
            anchors { left: parent.left; leftMargin: 20; right: parent.right; rightMargin: 20 }
            color: "white"
            wrapMode: Text.WordWrap
            font {
//                family: localFont.name;
                pixelSize: 12;
                bold: false }
            text: interpreter.lastError ;
            //+ "\nReal Code\n" + interpreter.effectiveSource
        }
    }
}
