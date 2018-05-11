import QtQuick 2.2
import QtQml.Models 2.1
import QtQuick.Controls 1.0
import Studio.Timeline 1.0
import QtGraphicalEffects 1.0
import QtQuick.Window 2.2
import 'Timeline.js' as TimelineLogic

Rectangle {
    id: root
    SystemPalette { id: activePalette }
    color: activePalette.window

    signal clipClicked()

    function setZoom(value) {
        toolbar.scaleSlider.value = value
        for (var i = 0; i < tracksRepeater.count; i++)
            tracksRepeater.itemAt(i).redrawWaveforms()
    }

    function adjustZoom(by) {
        setZoom(toolbar.scaleSlider.value + by)
    }

    function zoomIn() {
        adjustZoom(0.0625)
    }

    function zoomOut() {
        adjustZoom(-0.0625)
    }

    function resetZoom() {
        setZoom(1.0)
    }

    function zoomByWheel(wheel) {
        if (wheel.modifiers & Qt.ControlModifier) {
            adjustZoom(wheel.angleDelta.y / 720)
        }
        if (wheel.modifiers & Qt.ShiftModifier) {
            timelinetracks.trackHeight = Math.max(30, timelinetracks.trackHeight + wheel.angleDelta.y / 5)
        }
    }

    function makeTracksTaller() {
        timelinetracks.trackHeight += 20
    }

    function makeTracksShorter() {
        timelinetracks.trackHeight = Math.max(30, timelinetracks.trackHeight - 20)
    }  

    property int currentTrack: 0
    property color selectedTrackColor: Qt.rgba(0.8, 0.8, 0, 0.3);
    property alias trackCount: tracksRepeater.count
    property bool stopScrolling: false
    property color studioYellow: Qt.rgba(255/255, 215/255, 0/255, 1.0) 

    onCurrentTrackChanged: timeline.selection = []

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: menu.popup()
        onWheel: zoomByWheel(wheel)
    }

    DropArea {
        anchors.fill: parent
        onEntered: {
            
        }
        onExited: {

        }
        onPositionChanged: {
            
        }
        onDropped: {
            
        }
    }

    TimelineToolbar {
        id: toolbar
        width: parent.width
        height: ruler.height + 6
        anchors.top: parent.top
        z: 1
    }

    Row {
        anchors.top: toolbar.bottom
        MouseArea {
            id: tracksArea
            width: root.width
            height: root.height 
            focus: true
            hoverEnabled: true
            onClicked: timeline.position = (scrollView.flickableItem.contentX + mouse.x) / timelinetracks.scaleFactor
            onPositionChanged: {
                if (mouse.modifiers === Qt.ShiftModifier || mouse.buttons === Qt.LeftButton) {
                    timeline.position = (scrollView.flickableItem.contentX + mouse.x) / timelinetracks.scaleFactor
                }
            }

            Column {
                Flickable {
                    contentX: scrollView.flickableItem.contentX
                    width: root.width
                    height: ruler.height
                    interactive: false

                    Ruler {
                        id: ruler
                        width: tracksContainer.width
                        index: index
                        timeScale: timelinetracks.scaleFactor
                    }
                }
                ScrollView {
                    id: scrollView
                    width: root.width 
                    height: root.height - ruler.height - toolbar.height
        
                    Item {
                        width: tracksContainer.width 
                        height: trackHeaders.height + 30 // 30 is padding
                        Column {
                            // These make the striped background for the tracks.
                            // It is important that these are not part of the track visual hierarchy;
                            // otherwise, the clips will be obscured by the Track's background.
                            Repeater {
                                model: timelinetracks
                                delegate: Rectangle {
                                    width: tracksContainer.width
                                    color: (index === currentTrack)? selectedTrackColor : (index % 2)? activePalette.alternateBase : activePalette.base
                                    height: TimelineLogic.trackHeight(audio)
                                }
                            }
                        }
                        Column {
                            id: tracksContainer
                            Repeater { id: tracksRepeater; model: trackDelegateModel }
                        }
                    }
                }
            } 

            Rectangle {
                id: cursor
                visible: timeline.position > -1
                color: activePalette.text
                width: 1
                height: root.height - scrollView.__horizontalScrollBar.height - toolbar.height
                x: timeline.position * timelinetracks.scaleFactor - scrollView.flickableItem.contentX
                y: 0
            }
            TimelinePlayhead {
                id: playhead
                visible: timeline.position > -1
                x: timeline.position * timelinetracks.scaleFactor - scrollView.flickableItem.contentX - 5
                y: 0
                width: 11
                height: 5
            }
        }
    }

    Rectangle {
        id: dropTarget
        height: timelinetracks.trackHeight
        opacity: 0.5
        visible: false
        Text {
            anchors.fill: parent
            anchors.leftMargin: 100
            text: qsTr('Overwrite Track')
            style: Text.Outline
            styleColor: 'white'
            font.pixelSize: Math.min(Math.max(parent.height * 0.8, 15), 30)
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: bubbleHelp
        property alias text: bubbleHelpLabel.text
        color: application.toolTipBaseColor
        width: bubbleHelpLabel.width + 8
        height: bubbleHelpLabel.height + 8
        radius: 4
        states: [
            State { name: 'invisible'; PropertyChanges { target: bubbleHelp; opacity: 0} },
            State { name: 'visible'; PropertyChanges { target: bubbleHelp; opacity: 1} }
        ]
        state: 'invisible'
        transitions: [
            Transition {
                from: 'invisible'
                to: 'visible'
                OpacityAnimator { target: bubbleHelp; duration: 200; easing.type: Easing.InOutQuad }
            },
            Transition {
                from: 'visible'
                to: 'invisible'
                OpacityAnimator { target: bubbleHelp; duration: 200; easing.type: Easing.InOutQuad }
            }
        ]
        Label {
            id: bubbleHelpLabel
            color: application.toolTipTextColor
            anchors.centerIn: parent
        }
        function show(x, y, text) {
            bubbleHelp.x = x + tracksArea.x - scrollView.flickableItem.contentX - bubbleHelpLabel.width
            bubbleHelp.y = y + tracksArea.y - scrollView.flickableItem.contentY - bubbleHelpLabel.height
            bubbleHelp.text = text
            if (bubbleHelp.state !== 'visible')
                bubbleHelp.state = 'visible'
        }
        function hide() {
            bubbleHelp.state = 'invisible'
            bubbleHelp.opacity = 0
        }
    }
    DropShadow {
        source: bubbleHelp
        anchors.fill: bubbleHelp
        opacity: bubbleHelp.opacity
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8
        color: '#80000000'
        transparentBorder: true
        fast: true
    }

    Menu {
        id: menu  
        MenuItem {
            enabled: timelinetracks.trackHeight >= 50
            text: qsTr('Make Tracks Shorter')
            shortcut: 'Ctrl+-'
            onTriggered: makeTracksShorter()
        }
        MenuItem {
            text: qsTr('Make Tracks Taller')
            shortcut: 'Ctrl+='
            onTriggered: makeTracksTaller()
        }
        MenuItem {
            text: qsTr('Show Audio Waveforms')
            checkable: true
            checked: settings.timelineShowWaveforms
            onTriggered: {
                if (checked) {
                    if (settings.timelineShowWaveforms) {
                        settings.timelineShowWaveforms = checked
                        for (var i = 0; i < tracksRepeater.count; i++)
                            tracksRepeater.itemAt(i).redrawWaveforms()
                    } else {
                        settings.timelineShowWaveforms = checked
                        for (var i = 0; i < tracksRepeater.count; i++)
                            tracksRepeater.itemAt(i).remakeWaveforms(false)
                    }
                } else {
                    settings.timelineShowWaveforms = checked
                }
            }
        }
        MenuItem {
            text: qsTr('Show Video Thumbnails')
            checkable: true
            checked: settings.timelineShowThumbnails
            onTriggered: settings.timelineShowThumbnails = checked
        }
        MenuItem {
            text: qsTr('Reload')
            onTriggered: {
                timelinetracks.reload()
            }
        }
        onPopupVisibleChanged: {
            if (visible && application.OS === 'Windows' && __popupGeometry.height > 0) {
                // Try to fix menu running off screen. This only works intermittently.
                menu.__yOffset = Math.min(0, Screen.height - (__popupGeometry.y + __popupGeometry.height + 40))
                menu.__xOffset = Math.min(0, Screen.width - (__popupGeometry.x + __popupGeometry.width))
            }
        }
    }

    DelegateModel {
        id: trackDelegateModel
        model: timelinetracks
        Track {
            model: timelinetracks
            rootIndex: trackDelegateModel.modelIndex(index)
            height: TimelineLogic.trackHeight(audio)
            isAudio: audio
            isCurrentTrack: currentTrack === index
            timeScale: timelinetracks.scaleFactor
            selection: timeline.selection
            onClipClicked: {
                currentTrack = track.DelegateModel.itemsIndex
                timeline.selection = [ clip.DelegateModel.itemsIndex ];
                root.clipClicked()
            }
            onClipDragged: {
                // This provides continuous scrolling at the left/right edges.
                if (x > scrollView.flickableItem.contentX + scrollView.width - 50) {
                    scrollTimer.item = clip
                    scrollTimer.backwards = false
                    scrollTimer.start()
                } else if (x < 50) {
                    scrollView.flickableItem.contentX = 0;
                    scrollTimer.stop()
                } else if (x < scrollView.flickableItem.contentX + 50) {
                    scrollTimer.item = clip
                    scrollTimer.backwards = true
                    scrollTimer.start()
                } else {
                    scrollTimer.stop()
                }
                // Show distance moved as time in a "bubble" help.
                var track = tracksRepeater.itemAt(clip.trackIndex)
                var delta = Math.round((clip.x - clip.originalX) / timelinetracks.scaleFactor)
                var s = timeline.timecode(Math.abs(delta))
                // remove leading zeroes
                if (s.substring(0, 3) === '00:')
                    s = s.substring(3)
                s = ((delta < 0)? '-' : (delta > 0)? '+' : '') + s
                bubbleHelp.show(x, track.y + height, s)
            }
            onClipDropped: {
                scrollTimer.running = false
                bubbleHelp.hide()
            }
            onCheckSnap: {
                for (var i = 0; i < tracksRepeater.count; i++)
                    tracksRepeater.itemAt(i).snapClip(clip)
            } 
        }
    }
    
    Connections {
        target: timeline
        onPositionChanged: if (!stopScrolling) TimelineLogic.scrollIfNeeded()
        onDragging: TimelineLogic.dragging(pos, duration)
        onDropped: TimelineLogic.dropped()
        onDropAccepted: TimelineLogic.acceptDrop(xml)
        onSelectionChanged: { 

        }
    }

    Connections {
        target: timelinetracks
        onLoaded: toolbar.scaleSlider.value = Math.pow(timelinetracks.scaleFactor - 0.01, 1.0 / 3.0)
        onScaleFactorChanged: TimelineLogic.scrollIfNeeded()
    }
 
    Timer {
        id: scrollTimer
        interval: 25
        repeat: true
        triggeredOnStart: true
        property var item
        property bool backwards
        onTriggered: {
            var delta = backwards? -10 : 10
            if (item) item.x += delta
            scrollView.flickableItem.contentX += delta
            if (scrollView.flickableItem.contentX <= 0)
                stop()
        }
    }
}
