import QtQuick 2.0

EditorControl {
    Item {
        width: 30
        height: 30
        x: 80
        y: 20
        MouseArea {
            anchors.fill: parent
            onClicked: setValue(!(getValue()=="true"))
        }
        Image {
            source: "images/toolbar-button-off-hover.png"
            visible: targetEditor?targetEditor.selectedText != "true" : false
        }
        Image {
            source: "images/toolbar-button-on-hover.png"
            visible: targetEditor?targetEditor.selectedText == "true" : false
        }
    }
}
