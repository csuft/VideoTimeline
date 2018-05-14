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
    property int originalX: x
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

    border.color: selected? 'yellow' : 'black'
    border.width: 1
    // If clipping is enabled, an item will clip its own painting, 
    // as well as the painting of its children, to its bounding rectangle.
    clip: true 
    Drag.active: mouseArea.drag.active
    Drag.proposedAction: Qt.MoveAction
    opacity: Drag.active? 0.5 : 1.0

    function getColor() {
        return isAudio? 'darkseagreen' : root.studioYellow
    } 

    function generateWaveform() {
        if (!waveform.visible) return
        // This is needed to make the model have the correct count.
        // Model as a property expression is not working in all cases.
        waveformRepeater.model = Math.ceil(waveform.innerWidth / waveform.maxWidth)
        for (var i = 0; i < waveformRepeater.count; i++)
            waveformRepeater.itemAt(0).update()
    }

    onAudioLevelsChanged: generateWaveform()

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
            root.stopScrolling = true
            originalX = parent.x
            originalTrackIndex = trackIndex
            originalClipIndex = index
            startX = parent.x
            clipRoot.forceActiveFocus();
            clipRoot.clicked(clipRoot)
        }
        onPositionChanged: {
            parent.dragged(clipRoot, mouse)
        }
        onReleased: {
            root.stopScrolling = false
            parent.y = 0
            var delta = parent.x - startX
            if (Math.abs(delta) >= 1.0 || trackIndex !== originalTrackIndex) {
                parent.moved(clipRoot)
                originalX = parent.x
                originalTrackIndex = trackIndex
            } else {
                parent.dropped(clipRoot)
            }
        }
        onDoubleClicked: timeline.position = clipRoot.x / timelinetracks.scaleFactor
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
                root.stopScrolling = true
                startX = mapToItem(null, x, y).x
                originalX = 0 // reusing originalX to accumulate delta for bubble help
                parent.anchors.left = undefined
            }
            onReleased: {
                root.stopScrolling = false
                parent.anchors.left = clipRoot.left
                clipRoot.trimmedIn(clipRoot)
                parent.opacity = 0
            }
            onPositionChanged: {
                if (mouse.buttons === Qt.LeftButton) {
                    var newX = mapToItem(null, x, y).x
                    var delta = Math.round((newX - startX) / timeScale)
                    if (Math.abs(delta) > 0) {
                        if (clipDuration + originalX + delta > 0)
                            originalX += delta
                        clipRoot.trimmingIn(clipRoot, delta, mouse)
                        startX = newX
                    }
                }
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
                root.stopScrolling = true
                duration = clipDuration
                originalX = 0 // reusing originalX to accumulate delta for bubble help
                parent.anchors.right = undefined
            }
            onReleased: {
                root.stopScrolling = false
                parent.anchors.right = clipRoot.right
                clipRoot.trimmedOut(clipRoot)
            }
            onPositionChanged: {
                if (mouse.buttons === Qt.LeftButton) {
                    var newDuration = Math.round((parent.x + parent.width) / timeScale)
                    var delta = duration - newDuration
                    if (Math.abs(delta) > 0) {
                        if (clipDuration - originalX - delta > 0)
                            originalX += delta
                        clipRoot.trimmingOut(clipRoot, delta, mouse)
                        duration = newDuration
                    }
                }
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
            if (visible && application.OS !== 'OS X' && __popupGeometry.height > 0) {
                // Try to fix menu running off screen. This only works intermittently.
                menu.__yOffset = Math.min(0, Screen.height - (__popupGeometry.y + __popupGeometry.height + 40))
                menu.__xOffset = Math.min(0, Screen.width - (__popupGeometry.x + __popupGeometry.width))
            }
        }
    }
}
