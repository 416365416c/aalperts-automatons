import QtQuick 2.0

EditorControl {
    property int startVal: 0
    property int factor: 1
    onTargetEditorChanged: startVal = getValue();
    Image {
        width: 240
        source: "images/slider.png"
        Image {
            source: "images/slider-knob.png"
            property int startX: 110
            x: startX
            y: 12
            onXChanged: setValue(startVal + (x - startX) * factor)
            MouseArea {
                anchors.fill: parent
                drag.target: parent
                drag.axis: Drag.XAxis
            }
        }
    }
}
