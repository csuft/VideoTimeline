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
            action: pasteAction
            implicitWidth: 28
            implicitHeight: 24
        }

        ToolButton {
            action: copyAction
            implicitWidth: 28
            implicitHeight: 24
        } 

        ToolButton {
            action: removeAction
            implicitWidth: 28
            implicitHeight: 24
        } 

        ToolButton {
            action: splitAction
            implicitWidth: 28
            implicitHeight: 24
        } 

        ToolButton {
            id: snapButton
            implicitWidth: 28
            implicitHeight: 24
            checkable: true 
            iconName: 'snap'
            iconSource: 'qrc:///images/images/snap.png'
            tooltip: qsTr('Toggle snapping')
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
        id: pasteAction
        tooltip: qsTr('Paste - Paste the current clip to the end of track')
        iconName: 'edit-paste'
        iconSource: 'qrc:///images/images/audio-meter.png'
        enabled: TimelineWidget.selection
        onTriggered: TimelineWidget.removeSelection(true)
    }

    Action {
        id: removeAction
        tooltip: qsTr('Remove current clip')
        iconName: 'list-remove'
        iconSource: 'qrc:/list-remove.png'
        onTriggered: TimelineWidget.remove(currentTrack, TimelineWidget.selection)
   }

    Action {
        id: copyAction
        tooltip: qsTr('Copy - Copy the current clip')
        iconName: 'edit-copy'
        iconSource: 'qrc::///images/images/audio-volume-muted.png' 
        onTriggered: TimelineWidget.copyClip(currentTrack, TimelineWidget.selection)
    }

    Action {
        id: splitAction
        tooltip: qsTr('Split At Playhead')
        iconName: 'slice'
        iconSource: 'qrc:///images/images/slice.png'
        onTriggered: TimelineWidget.splitClip(currentTrack)
    }

    Action {
        id: zoomOutAction
        tooltip: qsTr("Zoom timeline out (-)")
        iconName: 'zoom-out'
        iconSource: 'qrc:///images/images/zoom-out.png'
        onTriggered: root.zoomOut()
    }

    Action {
        id: zoomInAction
        tooltip: qsTr("Zoom timeline in (+)")
        iconName: 'zoom-in'
        iconSource: "qrc:///images/images/zoom-out.png"
        onTriggered: root.zoomIn()
    }
}
