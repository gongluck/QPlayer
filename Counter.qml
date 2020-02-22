import QtQuick 2.0

//计数器
Item {
    property int number : 0
    function getNext(){
        return ++number
    }
}
