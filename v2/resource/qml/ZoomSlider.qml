import QtQuick 2.2
import QtQuick.Controls 1.0

Rectangle {
    property alias value: slider.value
    SystemPalette { id: activePalette }

    color: activePalette.window
    width: 200
    height: 24

    Slider {
        id: slider
        orientation: Qt.Horizontal
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            leftMargin: 4
            rightMargin: 4
        }
        minimumValue: 0
        maximumValue: 3.0
        value: 1
        function setScaleFactor() {
            
        }
        onValueChanged: {
            if (!pressed && typeof timelinetracks.scaleFactor != 'undefined')
                setScaleFactor()
        }
        onPressedChanged: { 
        }
    }
}
