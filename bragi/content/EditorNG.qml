import QtQuick 2.0
import Bragi 0.1 as Bragi
import "uglyStuff.js" as Magic

TextEdit {
    id: container
    color: "#6B6959"
    font { family: localFont.name; pixelSize: 12; bold: true }
    selectByMouse: true
    cursorDelegate: Rectangle {
        color: "white"
        width: 2
        SequentialAnimation on opacity {
            loops: -1
            NumberAnimation { from: 1.0; to: 0.0; }
            NumberAnimation { from: 0.0; to: 1.0; }
        }
    }
    //syntaxHighlighter: hi
    Bragi.SyntaxHighlighter{id: hi}
    text: "import QtQuick 2.0;\n\nRectangle {\n    width: 320;\n    height:480;\n    color:'lightsteelblue';\n}"

    property string lastText: ""
    onTextChanged: { indentify(); lastText = text; }
    //onWordSelected: TODO - would be a lot more robust and could remove the ignoreNext hack
    onSelectedTextChanged: considerCustomEditor();
    // REAL-TIME PROPERTY EDIT CODE
    property bool ignoreNext: false
    function ignoreNextSelection() { ignoreNext = true; }
    property Item editorControl: null
    function isBool(text) {
        return text == "true" || text == "false";
    }
    function isNumber(text) {
        return !isNaN(Number(text))
    }
    function isColor(text) {
        return false //TODO: Not implemented
    }
    function considerCustomEditor() {
        if (ignoreNext) {
            ignoreNext = false;
            return;
        }
        if (selectedText == '') {
            //console.log( "Editor removed!" )
            if (editorControl)
                editorControl.destroy();
            editorControl = null;
        } else if (editorControl == null) {
            if (isNumber(selectedText)) {
                editorControl = sliderComponent.createObject(container);
            } else if (isBool(selectedText)) {
                editorControl = checkComponent.createObject(container);
            } else if (isColor(selectedText)) {
                editorControl = paletteComponent.createObject(container);
            } else {
                return;
            }
            editorControl.targetEditor = container
            editorControl.targetCursor = container.selectionStart
            var posRect = container.positionToRectangle(container.selectionStart)
            editorControl.x = posRect.x - 100
            editorControl.y = posRect.y - 50
        } else {
            //console.log( "Editor in action!" )
        }
    }

    Component {
        id: paletteComponent
        EditorControl {
            Rectangle {
                color: "red"
                width: 40
                height: 40
            }
        }
    }

    Component {
        id: sliderComponent
        SliderControl {}
    }

    Component {
        id: checkComponent
        CheckboxControl {}
    }

    // INDENT CODE
    property bool iding: false
    property var indent: "    "
    function pyMply(str, cnt) { var ret = ""; for(var i=0; i<cnt; i++) ret += str; return ret; }
    function ownLine(str) {//NOTE: Due to some minor confusion, this function was accidentally written to be confusing.
        var acc=str.length;
        while (acc-- > 0 ) {
            if(str.charAt(acc) == '\n') return true;
            if(str.charAt(acc) >= 'A') return false;
        } 
        return true
    }
    function indentify() { //TODO: Cannot delete indention space - attempts should be recognized and delete the prev \n
        if (editorControl != null)
            return;//Skip when controls are up, otherwise it's too damn expensive
        //Auto writing a '\n}' like creator would feel more comfortable
        //TODO: Wouldn't it be nice to more easily find the difference?
        var insertPos = Magic.findInsertedOpenBrace(text, lastText);
        if (iding)
            return;
        iding = true
        var newText = ""
        var indentSteps = 0
        var acc = 0;
        for(var i=0; i<text.length; i++){
            var ch = text.charAt(i)
            if (ch == '\n') {
                var indentation = pyMply(indent, indentSteps);
                acc = indentation.length;
                newText += "\n" + indentation;
            } else {
                if (acc > 0) {
                    if (ch == ' ' || ch == '\t') {
                        acc--;
                        continue;
                    } else {
                        acc = 0;
                    }
                }
                if (ch == '{') {
                    indentSteps++;
                    if (i == insertPos) {
                        indentSteps--;
                        var indentation = pyMply(indent, indentSteps);
                        ch = '{\n'+indentation+'}';
                        //CursorPos futzing
                        cursorPosition -= (ch.length-1)
                    }
                }
                if (ch == '}') {
                    indentSteps--;
                    //Apply retroactively
                    var ownLine = true
                    var acc2=newText.length;
                    while (acc2-- > 0 ) {
                        if(str.charAt(acc2) == '\n') { break; }
                        if(str.charAt(acc2) >= 'A') { ownLine = false; break; }
                    } 
                    if (ownLine)
                        newText = newText.slice(0, newText.length - 4);
                }
                newText += ch;
            }
        }
        //console.log("A:" + text + "\n:::\nB:" + newText);
        if (newText != text) {
            var cursor = cursorPosition + (newText.length - text.length)
            text = newText
            while(cursor <= text.length && text.charAt(cursor) == ' ')
                cursor++
            cursorPosition = cursor;
        }
        iding = false
    }
}
