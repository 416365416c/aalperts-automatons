import Clam 0.2
import QtQuick 2.1
import QtQuick.Controls 1.0

ListView {
    id: view
    width: 320
    height: 480
    focus: true
    property Datum pending: null
    Connections {
        target: Qt.application
        onAboutToQuit: if (pending) pending.autoSave();
    }
    onCurrentItemChanged: {
        if (pending != null) {
            pending.autoSave();
            pending = null;
        }
        currentItem.seeYou();
    }
    //View follows arrow movement, and marks visible when scrolling
    preferredHighlightBegin: 0
    preferredHighlightEnd: view.height
    highlightRangeMode: root.scrollSelects ? ListView.StrictlyEnforceRange : ListView.ApplyRange
    highlightMoveDuration: 100
    delegate: Rectangle {
        id: container
        function seeYou() { 
            if(!modelData.seen) {
                view.pending = modelData;
                modelData.seen = true;
                root.numberSeen++;
            }
            notesEdit.forceActiveFocus();
        }
        width: ListView.view.width
        height: 240
        color: modelData.seen ? "lightsteelblue" : "goldenrod"
        MouseArea {
            anchors.fill: parent
            onClicked: { view.currentIndex = index; }
        }
        Item { id: subContainer //gives margins to the text areas
            anchors.fill: parent
            anchors.margins: 8
            TextArea {
                height: parent.height
                width: container.ListView.isCurrentItem ? parent.width / 2 : 2*parent.width / 3
                Behavior on width { NumberAnimation {} }//Can text handle this? :P
                text: sha1 + "\n" + subject + " <" + author + ">\n" + body
                readOnly: true
            }
            TextArea {
                id: notesEdit
                height: parent.height
                width: container.ListView.isCurrentItem ? parent.width / 2 : parent.width / 3
                x: container.ListView.isCurrentItem ? parent.width / 2 : 2*parent.width / 3
                Behavior on width { NumberAnimation {} }//Can text handle this? :P
                Behavior on x{ NumberAnimation {} }
                //placeholderText: "Your note goes here"
                text: note
                onActiveFocusChanged: { if(notesEdit.activeFocus) view.currentIndex = index }
                onTextChanged: {
                    if (text.search(';') != -1)
                        text = "No semicolons allowed!"
                    modelData.note = text;
                    view.pending = modelData;
                }
            }
        }
    }
}
