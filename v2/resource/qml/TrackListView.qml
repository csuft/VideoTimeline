import QtQuick 2.2
import QtQuick.Controls 1.0
import Studio.Timeline 1.0
import QtGraphicalEffects 1.0
import QtQml.Models 2.2
import QtQuick.Window 2.2

//  Component instances can directly access the component scopes of their ancestors.
MouseArea {
    id: clipRoot
    property string name: ''
    property string source: '' 
    property int inPoint: 0
    property int outPoint: 0
    property int duration: 0 
    property double frameRate: 30
    property bool isAudio: false  
    property var audioLevels 
    property int trackIndex 
    property int clipIndex
    property bool selected: false
    
    signal clicked(var clip)
    signal moved(var clip)
    signal dragged(var clip, var mouse)
    signal dropped(var clip) 
    signal trimmingIn(var clip, real delta, var mouse)
    signal trimmedIn(var clip)
    signal trimmingOut(var clip, real delta, var mouse)
    signal trimmedOut(var clip)

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
    onAudioLevelsChanged: generateAudioWaves()

    function getColor() {
        return isAudio? 'darkseagreen' : 'green'
    }  

    function imagePath(time) {
        return ''
    }

    function generateAudioWaves() {
        if (!waveform.visible) return
        waveRepeater.model = Math.ceil(waveform.innerWidth/waveform.maxWidth)
        for (var i = 0; i < waveRepeater.count; ++i) {
            waveRepeater.itemAt(0).update()
        }
    }

    // If clipping is enabled, an item will clip its own painting, 
    // as well as the painting of its children, to its bounding rectangle.
    clip: true 
    opacity: Drag.active? 0.8 : 1.0
    drag.target: icon

    Rectangle {
        id: icon
        width: parent.width
        height: parent.height
        anchors {
            horizontalCenter: parent.horizontalCenter;
            verticalCenter: parent.verticalCenter
        }
        radius: 3
        border.color: selected? 'red' : 'black'
        border.width: 1

        Drag.active: clipRoot.drag.active
        Drag.source: clipRoot
        Drag.hotSpot.x: 30
        Drag.hotSpot.y: 30

        states: [
            State {
                when: icon.Drag.active
                ParentChange {
                    target: icon
                    parent: root
                }

                AnchorChanges {
                    target: icon;
                    anchors.horizontalCenter: undefined;
                    anchors.verticalCenter: undefined
                }
            },
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
            //source: imagePath(outPoint)
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
            //source: imagePath(inPoint)
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

            Repeater {
                id: waveRepeater
                TimelineWaveform {
                    width: Math.min(waveform.innerWidth, waveform.maxWidth)
                    height: waveform.height
                    fillColor: getColor()
                    property int channels: 2
                    inPoint: Math.round(clipRoot.inPoint + index * waveform.maxWidth/timeScale)*channels
                    outPoint: inPoint + Math.round(width/timeScale)*channels
                    levels: audioLevels
                }
            }
        }

        // peak line
        Rectangle { 
            width: parent.width - parent.border.width * 2
            visible: !isAudio
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
            anchors.leftMargin: parent.border.width
            width: label.width + 2
            height: label.height
        }

        Text {
            id: label
            text: name
            visible: true
            font.pointSize: 8
            anchors {
                top: parent.top
                left: parent.left
                topMargin: parent.border.width + 1
                leftMargin: parent.border.width + 1
            }
            color: 'black'
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
    }

    DropArea {
        anchors {fill: parent}
        onEntered: {
            console.log("dragged", drag.source.clipIndex)
            console.log("moved", delegateRoot.clipIndex)
            trackModel.items.move(drag.source.clipIndex, clipRoot.clipIndex)
            // notify model update
            
        }
    } 

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        propagateComposedEvents: true
        cursorShape: Qt.ArrowCursor
        onClicked: menu.show()
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