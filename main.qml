import QtQuick 2.12
import QtQuick.Window 2.12
import QtMultimedia 5.12

Window {
    visible: true
    width: 640
    height: 480
    title: mediaplayer.source

    TextInput {
        id: uri
        width: parent.width
        text: "D://code//RandB//media//gx.wmv"
        color: "blue"
        focus: true

        onEditingFinished: {
           mediaplayer.play()
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
