import QtQuick 2.12
import QtQuick.Window 2.12
import QtMultimedia 5.12

Window {
    visible: true
    width: 640
    height: 480
    title: uri.text

    TextInput {
        id: uri
        width: parent.width
        text: "D:\\code\\RandB\\media\\gx.wmv"
        color: "blue"
        //focus: true

        onEditingFinished: {
            player.play(uri.text)
        }
    }

    Rectangle {
        id: displaywnd
        objectName: "displaywnd"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height - uri.height
        color: "black"
    }
}
