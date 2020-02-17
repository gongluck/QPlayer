import QtQuick 2.12
import QtQuick.Window 2.12
import QtMultimedia 5.12
import QtQuick.Controls 2.12

Rectangle {
    TextInput {
        id: uri
        width: parent.width
        text: "file://video.webm"
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
