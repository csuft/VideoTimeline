import QtQuick 2.2
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

ToolBar { 
    property color checkedColor: Qt.rgba(activePalette.highlight.r, activePalette.highlight.g, activePalette.highlight.b, 0.3)
    property alias scaleSlider: scaleSlider

    SystemPalette { id: activePalette }

    width: 200
    height: 24
    anchors.margins: 0

    style: ToolBarStyle {
        background: Rectangle {
            anchors.fill: parent
            color: "black"
        }
    }

    RowLayout {
        ToolButton {
            action: addAction
            implicitWidth: 28
            implicitHeight: 24
        } 

        ToolButton {
            action: copyAction
            implicitWidth: 28
            implicitHeight: 24
        } 

        ToolButton {
            action: cutAction
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
        id: addAction 
        tooltip: qsTr('Add a new clip video track')
        iconSource: 'qrc:///images/images/audio-meter.png'
        onTriggered: TimelineWidget.addClip(currentTrack)
    }
     
    Action {
        id: removeAction
        tooltip: qsTr('Remove current clip')
        iconSource: 'qrc:///images/images/edit-cut.png'
        onTriggered: TimelineWidget.removeClip(currentTrack, TimelineWidget.selection)
    }

    Action {
        id: cutAction
        tooltip: qsTr('Cut current clip')
        iconSource: 'qrc:///images/images/edit-cut.png'
        onTriggered: TimelineWidget.cutClip(currentTrack, TimelineWidget.selection)
    }

    Action {
        id: copyAction
        tooltip: qsTr('Copy the current clip to the end of the current track')
        iconSource: 'qrc:///images/images/object-locked.png' 
        onTriggered: TimelineWidget.copyClip(currentTrack, TimelineWidget.selection)
    } 

    Action {
        id: splitAction
        tooltip: qsTr('Split At Playhead')
        iconSource: 'qrc:///images/images/slice.png'
        onTriggered: TimelineWidget.splitClip(currentTrack)
    }

    Action {
        id: zoomOutAction
        tooltip: qsTr("Zoom timeline out (-)")
        iconSource: 'qrc:///images/images/zoom-out.png'
        onTriggered: root.zoomOut()
    }

    Action {
        id: zoomInAction
        tooltip: qsTr("Zoom timeline in (+)")
        iconSource: "qrc:///images/images/zoom-in.png"
        onTriggered: root.zoomIn()
    }
}
