import QtQuick 2.12
import QtQuick.Window 2.12
import QtMultimedia 5.12
import QtQuick.Controls 2.12

Rectangle {
    color: "black"

    function setTop(){
        z = 1
    }
    function setBot(){
        z = 0
    }

    TextInput {
        id: uri
        width: parent.width
        text: "E://code/RandB/media/gx.wmv"
        color: "blue"
        clip: true
//        onEditingFinished: {
//           mediaplayer.play()
//        }
    }

    MediaPlayer {
        id: mediaplayer
        source: uri.text
    }
    VideoOutput {
        id: videooutput
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height - uri.height
        source: mediaplayer
        autoOrientation: true 
    }

    MouseArea{
        anchors.fill: videooutput
        onClicked: {
            mediaplayer.play()
        }
        onDoubleClicked: {
            mediaplayer.stop()
        }

        property real lastX: 0
        property real lastY: 0
        onPressed: {
            lastX = mouseX
            lastY = mouseY
            setTop()
        }
        onReleased: {
            setBot()
        }

        onPositionChanged: {
            if (pressed) {
                parent.x += mouseX - lastX
                parent.y += mouseY - lastY
            }
        }
    }
}
