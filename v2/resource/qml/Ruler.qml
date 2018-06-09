import QtQuick 2.0
import QtQuick.Controls 1.0

Rectangle { 
    property real refFrameRate: TimelineModel.referenceFrameRate
    property int stepInterval: TimelineModel.stepInterval
    readonly property int majorTick: 14
    readonly property int minorTick: 8
    id: rulerTop 
    height: 30

    Repeater {
        model: parent.width / stepInterval
        Rectangle {
            anchors.bottom: rulerTop.bottom
            height:  index % TimelineWidget.visibleTickStep == 0 ? majorTick : minorTick
            width: 1 
            x: index * stepInterval
            color: 'black'
            Label {  
                anchors.bottom: parent.top
                anchors.bottomMargin: 2  
                anchors.horizontalCenter: parent.horizontalCenter
                text: TimelineWidget.timecode(index * referenceFrameRate / TimelineModel.tickTimeFactor)
                visible: index % TimelineWidget.visibleTickStep == 0 ? true : false
                font.pointSize: 6
                font.family: "Microsoft Yahei"  
            }
        }
    }
}
