import QtQuick 2.0
import QtQuick.Controls 1.0

Rectangle {
    property int index: 100
    property real timeScale: 1.0
    property int stepSize: Math.round(135 * Math.max(1.0, timeScale)) 

    id: rulerTop 
    height: 30

    Repeater {
        model: parent.width / stepSize
        Rectangle {
            anchors.bottom: rulerTop.bottom
            height: 18
            width: 1
            color: "steelblue"
            x: index * stepSize
            Label { 
                anchors.leftMargin: 2
                anchors.bottom: parent.top
                anchors.bottomMargin: 2
                color: "steelblue"
                x: index * stepSize + 2
                text: timeline.timecode(index * stepSize / timeScale)
                font.pointSize: 7.5
            }
        }
    }
}
