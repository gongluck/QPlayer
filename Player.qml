import QtQuick 2.12
import QtMultimedia 5.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2

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

    //背景图
    Image{
        id: bkimg
        source: "qrc:/bk.png"
        anchors.fill: parent
    }

    TextInput {
        id: uri
        width: parent.width - btn.width
        height: 25
        font.pixelSize: 15
        topPadding: 5
        //text: "file:../RandB/media/gx.wmv"
        text: qsTr("rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov")
        color: "blue"
        clip: true
        //onEditingFinished: {
        //  mediaplayer.play()
        //}
    }
    Button {
        id: btn
        width: 100
        anchors.right: parent.right
        height: uri.height
        text: qsTr("file")

        highlighted: true
        onClicked: {
            fileDialog.open()
        }
    }
    FileDialog {
            id: fileDialog
            title: qsTr("Please choose a media file")
            nameFilters: [ "Media Files (*.mp4 *.flv *.avi *.wmv *.mkv)", "*.*"]
            onAccepted: {
                uri.text = String(fileUrl)
            }
        }

    //需要安装LAVFilter
    //低版本QT(5.12)也可能会出现debug版本运行出错
    MediaPlayer {
        id: mediaplayer
        loops: MediaPlayer.Infinite
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
            bkimg.visible = false
            mediaplayer.source = uri.text
            mediaplayer.play()
        }
        onDoubleClicked: {
            mediaplayer.stop()
            bkimg.visible = true
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
