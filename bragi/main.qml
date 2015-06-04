import QtQuick 2.0
import "content" as Bragi

Rectangle {
    id: root
    width: 1280
    height: 1024
    color: "#272822"
    FontLoader { id: localFont; source: "content/fonts/VeraMoBd.ttf";
    onNameChanged: console.log("Name:"+name); Component.onCompleted:console.log("Name:"+name); }

    Item {
        id: mainArea
        anchors.topMargin: toolbar.height
        anchors.fill: parent
        Bragi.Canvas {
            id: canvas
            source: qmlEditor.text
            height: parent.height
            anchors.left: parent.left
            anchors.right: splitter.left
            z: 3
        }
        Rectangle {
            id: splitter
            width: 41
            border.color: "#6b6959"
            color: "#272822"
            height: parent.height
            x: parent.width/2
            y: -1
            z: 4
        }
        Text {
            id: fakeLineNumbers
            color: "#6b6959"
            font { family: localFont.name; pixelSize: 12; bold: true }
            y: editorFlickable.y + 1 - editorFlickable.contentY 
            anchors.right: splitter.right
            anchors.rightMargin: 5
            width: 30
            horizontalAlignment: Text.AlignRight
            text: generateLineNumbers(qmlEditor.lineCount);
            function generateLineNumbers(n) {
                var ret = "";
                var i = 0;
                while (++i <= n)
                    ret += (i + "\n")
                return ret;
            }
            z: 5
        }
        Flickable {
            id: editorFlickable
            height: parent.height
            anchors.left: splitter.right
            anchors.right: parent.right
            anchors.leftMargin: 14
            contentWidth: qmlEditor.paintedWidth
            contentHeight: qmlEditor.height
            y: 4
            z: 6

            Bragi.EditorNG {
                id: qmlEditor
                text: "import QtQuick 2.0;\n\nRectangle {\n    width: 320;\n    height:480;\n    color:'steelblue';\n}"
                focus: true
                width: editorFlickable.width
            }
        }
    }

    BorderImage {
        id: toolbar
        width: parent.width
        height: 36
        source: "content/images/toolbar.png"
        border.left: 4
        border.right: 4
        Image {
            id: logo
            x: 10
            y: 6
            source: "content/images/logo-bragi.png"
        }
        Bragi.ToolButton {
            id: scene
            width: 32
            height: 32
            anchors.left: logo.right
            anchors.leftMargin: 40
            label: ""
            icon: "content/images/icon-scene-on.png"
        }
        Bragi.ToolButton {
            id: size
            width: 32
            height: 32
            anchors.left: scene.right
            anchors.leftMargin: -1
            label: ""
            icon: "content/images/icon-size-off.png"
        }
        Bragi.ToolButton {
            id: grid
            width: 32
            height: 32
            anchors.left: size.right
            anchors.leftMargin: -1
            label: ""
            icon: "content/images/icon-grid-off.png"
        }
        Bragi.ToolButton {
            id: zoomIn
            width: 32
            height: 32
            anchors.left: grid.right
            anchors.leftMargin: 20
            label: ""
            onClicked: canvas.scale += .25
            icon: "content/images/icon-zoom-in-off.png"
        }
        Bragi.ToolButton {
            id: zoomOut
            width: 32
            height: 32
            anchors.left: zoomIn.right
            anchors.leftMargin: -1
            label: ""
            onClicked: canvas.scale -= .25
            icon: "content/images/icon-zoom-out-off.png"
        }
        Bragi.ToolButton {
            id: errorBanner
            width: 32
            height: 32
            anchors.left: zoomOut.right
            anchors.leftMargin: 20
            label: ""
            icon: "content/images/icon-error-banner-on.png"
        }
        Image {
            id: splitterHandle
            x: splitter.x
            source: "content/images/resize-handle.png"
            MouseArea {
                anchors.fill: parent
                property real dragMax: root.width - 240
                drag.target: splitter
                drag.axis: Drag.XAxis
                drag.minimumX: Math.min(canvas.itemWidth, dragMax)
                drag.maximumX: dragMax
            }
            z: 3
        }
        Bragi.ToolButton {
            id: snippet
            width: 32
            height: 32
            anchors.left: splitterHandle.right
            anchors.leftMargin: 10
            label: ""
            icon: "content/images/icon-snippet-off.png"
        }
        Bragi.ToolButton {
            id: documentation
            width: 32
            height: 32
            anchors.left: snippet.right
            anchors.leftMargin: -1
            label: ""
            icon: "content/images/icon-documentation-off.png"
        }
    }

    Image {
        id: statusbar
        source: "content/images/statusbar.png"
        fillMode: Image.TileHorizontally
        anchors.bottom: mainArea.bottom
        width: parent.width
        height: 20
        Text {
            text: Math.round(canvas.scale * 100) + "%"
            x: 20
            anchors.verticalCenter: parent.verticalCenter
            font {
//                family: localFont.name;
                pixelSize: 11;
                bold: true }
            color: "white"
        }
    }
}
