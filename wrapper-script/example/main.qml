import QtQuick 1.1
import MyApp 1.0

Rectangle {
    width: 300
    height: 300
    color: "lightsteelblue"
    QmlTestable {
        id: foo
        robjectName: "James"
        alpha: 7
        Component.onCompleted: console.log("Alpha: " + foo.alpha)
    }
    Text {
        anchors.centerIn: parent
        text: foo.beta ? foo.gamma : foo.robjectName
    }
    MouseArea{
        anchors.fill: parent
        onClicked: foo.beta = !foo.beta
    }
}
