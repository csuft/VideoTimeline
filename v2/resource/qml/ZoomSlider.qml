import QtQuick 2.2
import QtQuick.Controls 1.0

Rectangle {
    property alias value: slider.value
    property alias minimum: slider.minimumValue
    property alias maximum: slider.maximumValue
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
        maximumValue: 8.0
        value: 0
        function setScaleFactor() {
            var stepSize;
            if (value == 0.0) {stepSize = 2}
            else if (value == 1.0) { stepSize = 5}
            else if (value == 2.0) { stepSize = 10}
            else { stepSize = 15} 

            TimelineModel.scaleFactor = stepSize
        }
        onValueChanged: {
            if (!pressed && typeof TimelineModel.scaleFactor != 'undefined')
                setScaleFactor()
        }
        onPressedChanged: { 
            if (!pressed) {
                setScaleFactor()
                for (var i = 0; i < tracksRepeater.count, ++i) {
                    tracksRepeater.itemAt(i).redrawAudioWaves();
                }
            }
        }
    }
}
