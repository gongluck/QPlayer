import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480

    Counter{
        id : counter
    }

    Player {
        id:player1
        visible: true
        anchors.left:parent.left
        anchors.top:parent.top
        width: parent.width
        height: parent.height
    }

    Player {
        id:player2
        visible: true
        x:parent.x
        y:parent.height-height
        width: parent.width/3
        height: parent.height/3
        counter:counter
        canchangez : true
    }

    Player {
        id:player3
        visible: true
        x:player2.width
        y:parent.height-height
        width: parent.width/3
        height: parent.height/3
        counter:counter
        canchangez : true
    }

    Player {
        id:player4
        visible: true
        x:player2.width+player3.width
        y:parent.height-height
        width: parent.width/3
        height: parent.height/3
        counter:counter
        canchangez : true
    }
}
