import QtQuick 2.12
import QtQuick.Window 2.12
import QtMultimedia 5.12
import QtQuick.Controls 2.12

Rectangle {
    color: "black"

    TextInput {
        id: uri
        width: parent.width
        text: "E://code//RandB//media//gx.wmv"
        color: "blue"
        focus: true

        onEditingFinished: {
           mediaplayer.play()
        }
    }

    MouseArea{
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height - uri.height
        onPressedButtonsChanged: {
            mediaplayer.play()
        }

        anchors.fill: parent
        property real lastX: 0
        property real lastY: 0
        onPressed: {
            lastX = mouseX
            lastY = mouseY
        }
        onPositionChanged: {
            if (pressed) {
                parent.x += mouseX - lastX
                parent.y += mouseY - lastY
            }
        }
    }

    MediaPlayer {
        id: mediaplayer
        source: uri.text
    }
    VideoOutput {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height - uri.height
        source: mediaplayer
        autoOrientation: true 
    }
}
