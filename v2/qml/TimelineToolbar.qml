import QtQuick 2.2
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

ToolBar { 
    property color checkedColor: Qt.rgba(activePalette.highlight.r, activePalette.highlight.g, activePalette.highlight.b, 0.3)
    property alias scaleSlider: scaleSlider

    SystemPalette { id: activePalette }

    width: 200
    height: 24
    anchors.margins: 0

    RowLayout {
        ToolButton {
            action: menuAction
            implicitWidth: 28
            implicitHeight: 24
        }
        
        ToolButton {
            action: cutAction
            implicitWidth: 28
            implicitHeight: 24
        }

        ToolButton {
            action: copyAction
            implicitWidth: 28
            implicitHeight: 24
        }

        ToolButton {
            action: insertAction
            implicitWidth: 28
            implicitHeight: 24
        }

        ToolButton {
            action: deleteAction
            implicitWidth: 28
            implicitHeight: 24
        }
        ToolButton {
            action: overwriteAction
            implicitWidth: 28
            implicitHeight: 24
        }
        ToolButton {
            action: splitAction
            implicitWidth: 28
            implicitHeight: 24
        } 
        ToolButton {
            action: zoomOutAction
            implicitWidth: 28
            implicitHeight: 24
        }
        ZoomSlider {
            id: scaleSlider
        }
        ToolButton {
            action: zoomInAction
            implicitWidth: 28
            implicitHeight: 24
        }

        ColorOverlay {
            id: snapColorEffect
            visible: snapButton.checked
            anchors.fill: snapButton
            source: snapButton
            color: checkedColor
            cached: true
        }
    } 

    Action {
        id: cutAction
        tooltip: qsTr('Cut - Copy the current clip to the Source\nplayer and ripple delete it')
        iconName: 'edit-cut'
        iconSource: 'qrc:/edit-cut.png'
        enabled: timeline.selection.length
        onTriggered: timeline.removeSelection(true)
    }

    Action {
        id: copyAction
        tooltip: qsTr('Copy - Copy the current clip to the Source player (C)')
        iconName: 'edit-copy'
        iconSource: 'qrc:/edit-copy.png'
        enabled: timeline.selection.length
        onTriggered: timeline.copyClip(currentTrack, timeline.selection[0])
    }

    Action {
        id: insertAction
        tooltip: qsTr('Paste - Insert clip into the current track\nshifting following clips to the right (V)')
        iconName: 'edit-paste'
        iconSource: 'qrc:/edit-paste.png'
        onTriggered: timeline.insert(currentTrack)
    } 

    Action {
        id: overwriteAction
        tooltip: qsTr('Overwrite clip onto the current track')
        iconName: 'overwrite'
        iconSource: 'qrc:/overwrite.png'
        onTriggered: timeline.overwrite(currentTrack)
    }

    Action {
        id: splitAction
        tooltip: qsTr('Split At Playhead')
        iconName: 'slice'
        iconSource: 'qrc:/slice.png'
        onTriggered: timeline.splitClip(currentTrack)
    }

    Action {
        id: zoomOutAction
        tooltip: qsTr("Zoom timeline out (-)")
        iconName: 'zoom-out'
        iconSource: 'qrc:/zoom-out.png'
        onTriggered: root.zoomOut()
    }

    Action {
        id: zoomInAction
        tooltip: qsTr("Zoom timeline in (+)")
        iconName: 'zoom-in'
        iconSource: 'qrc:/zoom-in.png'
        onTriggered: root.zoomIn()
    }
}
