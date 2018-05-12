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
            id: snapButton
            implicitWidth: 28
            implicitHeight: 24
            checkable: true
            checked: true
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
        id: menuAction
        tooltip: qsTr('Display a menu of additional actions')
        iconName: 'format-justify-fill'
        iconSource: 'qrc:///images/images/sliderDrag.png'
        onTriggered: menu.popup()
    }
    
    Action {
        id: cutAction
        tooltip: qsTr('Cut - Copy the current clip to the Source\nplayer and ripple delete it')
        iconName: 'edit-cut'
        iconSource: 'qrc:///images/images/audio-meter.png'
        enabled: timeline.selection
        onTriggered: timeline.removeSelection(true)
    }

        Action {
        id: deleteAction
        tooltip: qsTr('Remove current clip\nshifting following clips to the left')
        iconName: 'list-remove'
        iconSource: 'qrc:/list-remove.png'
        onTriggered: timeline.remove(currentTrack, timeline.selection[0])
   }

    Action {
        id: copyAction
        tooltip: qsTr('Copy - Copy the current clip to the Source player (C)')
        iconName: 'edit-copy'
        iconSource: 'qrc::///images/images/audio-volume-muted.png'
        enabled: timeline.selection.length
        onTriggered: timeline.copyClip(currentTrack, timeline.selection[0])
    }

    Action {
        id: insertAction
        tooltip: qsTr('Paste - Insert clip into the current track\nshifting following clips to the right (V)')
        iconName: 'edit-paste'
        iconSource: 'qrc:///images/images/audio-volume-high.png'
        onTriggered: timeline.insert(currentTrack)
    } 

    Action {
        id: overwriteAction
        tooltip: qsTr('Overwrite clip onto the current track')
        iconName: 'overwrite'
        iconSource: 'qrc:///images/images/object-locked.png'
        onTriggered: timeline.overwrite(currentTrack)
    }

    Action {
        id: splitAction
        tooltip: qsTr('Split At Playhead')
        iconName: 'slice'
        iconSource: 'qrc:///images/images/slice.png'
        onTriggered: timeline.splitClip(currentTrack)
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
