import Clam 0.2
import QtQuick 2.1
import QtQuick.Window 2.1
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0

ApplicationWindow {
    id: root
    width: 800
    height: 800
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: "Change Log Automation Machine"

    property alias scrollSelects: selectionOption.checked
    property int numberSeen: 0
    function loadNumberSeen() {
        numberSeen = 0;
        for(var i=0; i<DataManager.data.length; i++)
            if(DataManager.data[i].seen)
                numberSeen++;
    }
    Component.onCompleted: loadNumberSeen();

    FileDialog {
        id: saveFileDialog
        title: "Choose a location to save output to"
        selectExisting: false
        onAccepted: DataManager.writeToFile(saveFileDialog.fileUrl, DataManager.collateNotes());
    }

    AboutDialog { id: about }

    TextEdit {
        id: magicClipper
        visible: false
        function go() {
            text = DataManager.collateNotes();
            cut();
        }
    }

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem { text: "Save Log to File..."; onTriggered: saveFileDialog.visible = true; }
            MenuItem { text: "Copy Log to Clipboard"; onTriggered: magicClipper.go(); }
            MenuItem { text: "Quit"; onTriggered: Qt.quit() }
        }
        Menu {
            title: "Options"
            MenuItem { 
                id: visualOption
                text: "Toggle Particle Visualizations"; 
                checkable: true
                checked: true
            }
            MenuItem { 
                id: selectionOption
                text: "Toggle Selection Follows Scroll";
                checkable: true
                checked: false
            }
        }
        Menu {
            title: "Help"
            MenuItem { text: "About..."; onTriggered: about.visible = true; }
        }
    }

    ScrollView {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: visuals.left
        anchors.margins: 8
        LogList {
            model: DataManager.data
        }
    }

    Text {
        anchors.centerIn: parent
        visible: DataManager.data.length == 0
        text: "Uh-oh! No data?\nDid you run this in a git repository with changes?"
    }

    ParticleIndicator {
        id: visuals
        running: visualOption.checked
        x: running ? parent.width -160 : parent.width
        Behavior on x { NumberAnimation {} }
        height: parent.height
        width: 160
        max: DataManager.data.length
        done: numberSeen
    }
}
