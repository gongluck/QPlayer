import QtQuick 2.12
import QtQuick.Window 2.12
import QtMultimedia 5.12
import QtQuick.Controls 2.12

//播放器
Rectangle {
    color: "black"
    property Counter counter
    property bool canchangez : false

    function setTop() {
        z = counter.getNext()
        //console.log("change z to ", z)
    }
    function setBot(){
        //z = 0
    }

    TextInput {
        id: uri
        width: parent.width
        //text: "file:../RandB/media/gx.wmv"
        text: "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov"
        color: "blue"
        clip: true
        //onEditingFinished: {
        //  mediaplayer.play()
        //}
    }

    //需要安装LAVFilter
    //低版本QT(5.12)也可能会出现debug版本运行出错
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
            if(canchangez){
                setTop()
            }
        }
        onReleased: {
            if(canchangez){
                setBot()
            }
        }

        onPositionChanged: {
            if (pressed) {
                parent.x += mouseX - lastX
                parent.y += mouseY - lastY
            }
        }
    }
}
