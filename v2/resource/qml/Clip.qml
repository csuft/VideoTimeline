import QtQuick 2.2
import QtQuick.Controls 1.0
import Studio.Timeline 1.0
import QtGraphicalEffects 1.0
import QtQml.Models 2.2
import QtQuick.Window 2.2

//  Component instances can directly access the component scopes of their ancestors.
Rectangle {
    id: clipRoot
    property string clipName: ''
    property string clipSource: '' 
    property int inPoint: 0
    property int outPoint: 0
    property int clipDuration: 0 
    property bool isAudio: false 
    property var audioLevels 
    property int trackIndex 
    property bool selected: false   

    signal clicked(var clip)
    signal moved(var clip)
    signal dragged(var clip, var mouse)
    signal dropped(var clip) 
    signal trimmingIn(var clip, real delta, var mouse)
    signal trimmedIn(var clip)
    signal trimmingOut(var clip, real delta, var mouse)
    signal trimmedOut(var clip)

    SystemPalette { id: activePalette }
    gradient: Gradient {
        GradientStop {
            id: gradientStop
            position: 0.00
            color: Qt.lighter(getColor())
        }
        GradientStop {
            id: gradientStop2
            position: 1.0
            color: getColor()
        }
    }

    border.color: selected? 'red' : 'black'
    border.width: 1
    // If clipping is enabled, an item will clip its own painting, 
    // as well as the painting of its children, to its bounding rectangle.
    clip: true 
    Drag.active: mouseArea.drag.active
    Drag.proposedAction: Qt.MoveAction
    opacity: Drag.active? 0.5 : 1.0

    function getColor() {
        return 'darkseagreen'
    }  

    Image {
        id: outThumbnail
        visible: false
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: parent.border.width
        anchors.rightMargin: parent.border.width + 1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height / 2
        width: height * 2.0
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: inThumbnail
        visible: false
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: parent.border.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height / 2
        width: height * 2.0
        fillMode: Image.PreserveAspectFit 
    } 

    Row {
        id: waveform
        visible: true
        height: isAudio? parent.height : parent.height / 2
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: parent.border.width
        opacity: 0.5
        property int maxWidth: 10000
        property int innerWidth: clipRoot.width - clipRoot.border.width * 2

    }

    Rectangle { 
        width: parent.width - parent.border.width * 2
        visible: true
        height: 1
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: parent.border.width
        anchors.bottomMargin: waveform.height * 0.9
        color: Qt.darker(parent.color)
        opacity: 0.5
    }

    Rectangle {
        // text background
        color: 'lightgray'
        visible: true 
        opacity: 0.5
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: parent.border.width
        anchors.leftMargin: parent.border.width +
            ((isAudio) ? 0 : inThumbnail.width)
        width: label.width + 2
        height: label.height
    }

    Text {
        id: label
        text: clipName
        visible: true 
        font.pointSize: 8
        anchors {
            top: parent.top
            left: parent.left
            topMargin: parent.border.width + 1
            leftMargin: parent.border.width +
                ((isAudio) ? 0 : inThumbnail.width) + 1
        }
        color: 'black'
    }

    states: [
        State {
            name: 'normal'
            when: !clipRoot.selected
            PropertyChanges {
                target: clipRoot
                z: 0
            }
        }, 
        State {
            name: 'selected'
            when: clipRoot.selected
            PropertyChanges {
                target: clipRoot
                z: 1
            }
            PropertyChanges {
                target: gradientStop
                color: Qt.darker(getColor())
            }
        }
    ]

    MouseArea {
        anchors.fill: parent
        enabled: true
        acceptedButtons: Qt.RightButton
        onClicked: menu.show()
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent 
        acceptedButtons: Qt.LeftButton
        drag.target: parent
        drag.axis: Drag.XAxis
        property int startX
        onPressed: {
            parent.clicked(clipRoot) 
        }
        onPositionChanged: {
            parent.dragged(clipRoot, mouse)
        }
        onReleased: {
            parent.dropped(clipRoot)
        }
        onDoubleClicked: TimelineWidget.position = clipRoot.x / TimelineModel.scaleFactor
        onWheel: zoomByWheel(wheel)

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            propagateComposedEvents: true
            cursorShape: Qt.ArrowCursor
            onClicked: menu.show()
        }
    } 

    Rectangle {
        id: trimIn
        enabled: true
        anchors.left: parent.left
        anchors.leftMargin: 0
        height: parent.height
        width: 5
        color: isAudio? 'green' : 'lawngreen'
        opacity: 0
        Drag.active: trimInMouseArea.drag.active
        Drag.proposedAction: Qt.MoveAction

        MouseArea {
            id: trimInMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.SizeHorCursor
            drag.target: parent
            drag.axis: Drag.XAxis
            property double startX

            onPressed: {

            }
            onReleased: {

            }
            onPositionChanged: {

            }
            onEntered: parent.opacity = 0.5
            onExited: parent.opacity = 0
        }
    }
    Rectangle {
        id: trimOut
        enabled: true 
        anchors.right: parent.right
        anchors.rightMargin: 0
        height: parent.height
        width: 5
        color: 'red'
        opacity: 0
        Drag.active: trimOutMouseArea.drag.active
        Drag.proposedAction: Qt.MoveAction

        MouseArea {
            id: trimOutMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.SizeHorCursor
            drag.target: parent
            drag.axis: Drag.XAxis
            property int duration

            onPressed: {

            }
            onReleased: {

            }
            onPositionChanged: {

            }
            onEntered: parent.opacity = 0.5
            onExited: parent.opacity = 0
        }
    }
    Menu {
        id: menu
        function show() {
            popup()
        }
        MenuItem {
            visible: true
            text: qsTr('Cut') 
        }
        MenuItem {
            visible: true 
            text: qsTr('Copy') 
        }
        MenuSeparator {
            visible: true 
        }
        MenuItem {
            text: qsTr('Remove') 
        }
        MenuSeparator {
            visible: true 
        }
        MenuItem {
            visible: true 
            text: qsTr('Split At Playhead') 
        }
        MenuItem {
            visible: true
            text: qsTr('Rebuild Audio Waveform') 
        }
        onPopupVisibleChanged: {
            if (visible && Qt.application.OS !== 'OS X' && __popupGeometry.height > 0) {
                // Try to fix menu running off screen. This only works intermittently.
                menu.__yOffset = Math.min(0, Screen.height - (__popupGeometry.y + __popupGeometry.height + 40))
                menu.__xOffset = Math.min(0, Screen.width - (__popupGeometry.x + __popupGeometry.width))
            }
        }
    }
}
