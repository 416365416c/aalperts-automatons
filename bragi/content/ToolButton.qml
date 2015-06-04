import QtQuick 2.0

BorderImage {

    property alias label: label.text
    property alias icon: iconImage.source
    signal clicked

    source: "images/toolbar-button-off.png"
    border.left: 5; border.right: 5
    width: label.width + 16
    height: parent.height - 4
    y: 3

    Text {
        id: label
//        font { family: localFont.name; pixelSize: 11; bold: true }
        color: "#ffffff"
        anchors.centerIn: parent
    }

    Image {
        id: iconImage
        x: 4
        y: 3
        source: "images/icon-blank.png"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: parent.clicked()
    }
}
