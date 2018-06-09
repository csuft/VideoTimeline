import QtQuick 2.0
import QtQuick.Controls 1.0

Rectangle { 
    property real refFrameRate: TimelineModel.referenceFrameRate
    property int tickInterval: TimelineModel.tickInterval
    readonly property int majorTick: 14
    readonly property int minorTick: 8
    id: rulerTop 
    height: 30  

    Repeater {
        model: parent.width / tickInterval
        Rectangle {
            anchors.bottom: rulerTop.bottom
            height:  index % 3 == 0 ? majorTick : minorTick
            width: 1 
            x: index * tickInterval
            color: 'black'
            Label {    
                anchors.bottom: parent.top
                anchors.bottomMargin: 2  
                anchors.horizontalCenter: parent.horizontalCenter
                text: TimelineWidget.timecode(index * TimelineModel.referenceFrameRate / TimelineModel.tickTimeFactor)
                visible: index % 3 == 0 ? true : false
                font.pointSize: 6
                font.family: "Microsoft Yahei"  
            }
        }
    }
}
