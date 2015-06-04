import QtQuick 2.0
import Bragi 0.1

Editor {
    id: container
    //Colors
    textColor: "#6B6959"
    selectionColor: "lightsteelblue"
    //Cursor
    Rectangle {
        color: "white"
        width: 2
        height: container.cursorRectangle.height + 4
        x: container.cursorRectangle.x - 1
        y: container.cursorRectangle.y - 2
        SequentialAnimation on opacity {
            loops: -1
            NumberAnimation { from: 1.0; to: 0.0; }
            NumberAnimation { from: 0.0; to: 1.0; }
        }
    }
    //Reimplementing UI Logic
    function goToLineStart() {
        while (cursorPosition > 0 && container.text[cursorPosition - 1] != '\n')
            cursorPosition--;
    }
    function goToLineEnd() {
        while (cursorPosition < container.text.length - 1 && container.text[cursorPosition] != '\n')
            cursorPosition++;
    }
    function moveLine(num) {
        var startPos = cursorPosition;
        goToLineStart();
        var offset = startPos - cursorPosition;
        while (num > 0) {
            goToLineEnd();
            cursorPosition++;
            num--;
        }
        while (num < 0) {
            cursorPosition--;
            goToLineStart();
            num++;
        }
        cursorPosition += offset;
    }
    MouseArea{
        anchors.fill: parent
        onClicked: {
            console.log("Whoa!" + container.positionAt(mouse.x, mouse.y));
            container.deselect();
            container.cursorPosition = container.positionAt(mouse.x, mouse.y);
            //TODO: MouseSelection
        }
        onDoubleClicked: {
            container.selectWord();
        }
    }
    Keys.onPressed: {
        //TODO: Key selection
        console.log("Wee!" + event.key);
        if (event.key == Qt.Key_Left) {
            container.cursorPosition--;
        } else if (event.key == Qt.Key_Right) {
            container.cursorPosition++;
        } else if (event.key == Qt.Key_Up) {
            moveLine(-1);
        } else if (event.key == Qt.Key_Down) {
            moveLine(1);
        } else if (event.key == Qt.Key_End) {
            goToLineEnd();
        } else if (event.key == Qt.Key_Home) {
            goToLineStart();
        } else if (event.key == Qt.Key_Backspace) {
            container.delPrev();
        } else if (event.key == Qt.Key_Delete) {
            container.del();
        } else {
            if (event.modifier == Qt.ControlModifier) {
                if (event.text == "a") {
                    container.selectAll();
                } else if (event.text == "z") {
                    container.undo();
                } else if (event.text == "y") {
                    container.redo();
                } else if (event.text == "x") {
                    container.cut();
                } else if (event.text == "v") {
                    container.paste();
                } else if (event.text == "c") {
                    container.copy();
                }
            } else {
                container.insert(event.text);
            }
        }
    }
}
