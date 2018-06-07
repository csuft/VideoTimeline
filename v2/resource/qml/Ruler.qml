import QtQuick 2.0
import QtQuick.Controls 1.0

Rectangle { 
    property real timeScale: TimelineModel.scaleFactor
    property int stepSize: TimelineModel.stepSize
    readonly property int majorTick: 14
    readonly property int minorTick: 8
    id: rulerTop 
    height: 30

    Repeater {
        model: parent.width / stepSize
        Rectangle {
            anchors.bottom: rulerTop.bottom
            height:  index % 3 == 0 ? majorTick : minorTick
            width: 1 
            x: index * stepSize
            color: 'black'
            Label {  
                anchors.bottom: parent.top
                anchors.bottomMargin: 2  
                anchors.horizontalCenter: parent.horizontalCenter
                text: TimelineWidget.timecode(index * 30  / timeScale)
                visible: index % 3 == 0 ? true : false
                font.pointSize: 6
                font.family: "Microsoft Yahei"  
            }
        }
    }
}
