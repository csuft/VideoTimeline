import QtQuick 2.2
import QtQml.Models 2.1
import QtQuick.Controls 2.3
import Studio.Timeline 1.0
import QtGraphicalEffects 1.0
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4 
import 'Timeline.js' as TimelineLogic

Rectangle {
    // system attributes
    id: root  
    color: '#232323' 

    function adjustZoom(by) {
        setZoom(by)
    }

    function zoomIn() { 
        adjustZoom(1.0)
    }

    function zoomOut() {
        adjustZoom(-1.0)
    }

    function resetZoom() {
        adjustZoom(0.0)
    }

    function zoomByWheel(wheel) {
        if (wheel.modifiers & Qt.ControlModifier) {
            adjustZoom(wheel.angleDelta.y / 720)
        }
        if (wheel.modifiers & Qt.ShiftModifier) {
            TimelineModel.trackHeight = Math.max(30, TimelineModel.trackHeight + wheel.angleDelta.y/5)
        }
    }

    function makeTracksTaller() {
        TimelineModel.trackHeight += 20
    }

    function makeTracksShorter() {
        TimelineModel.trackHeight = Math.max(30, TimelineModel.trackHeight - 20)
    } 

    // custom properties
    property int currentTrack: 0
    readonly property color selectedTrackColor: Qt.rgba(0.5, 0.5, 0, 0.3)
    readonly property color sutdioYellow: Qt.rgba(255/255, 215/255, 0/255, 1.0)  
    readonly property int padding: 0
    
    // signals
    signal clipClicked()

    // signal handlers
    onCurrentTrackChanged: {
        console.log("Current active track: ", currentTrack)
    }   

    Row {
         ScrollView {
            id: tracksScrollView
            width: root.width
            height: root.height
            contentWidth: scrollColumn.width
            contentHeight: scrollColumn.height
            Column { 
                id: scrollColumn
                Flickable {
                    contentX: tracksScrollView.contentItem.x
                    width: tracksBackground.width
                    height: ruler.height
                    interactive: false
                    Ruler {
                        id: ruler
                        width: tracksBackground.width
                        MouseArea {
                            id: rulerMouseArea
                            anchors.fill: parent 
                            acceptedButtons: Qt.LeftButton
                            onClicked: { 
                                TimelineWidget.position = mouse.x  
                            } 
                        }   
                    }
                }  
                                     
                // tracks
                Column {
                    id: tracksBackground 
                    Repeater {
                        id: tracksRepeater
                        model: tracksModel 
                    } 
                }
                
            } 

            Item {
                id: cursorItem 
                width: playhead.width
                height: playhead.height + cursor.height
                x:TimelineWidget.position - tracksScrollView.contentItem.x - 5
                y:0
                Rectangle {
                    id: cursor
                    visible: tracksBackground.width > 0
                    color: '#707070'
                    width: 1
                    height: tracksScrollView.height + padding
                    x: 5
                    y: 0
                }

                TimelinePlayhead {
                    id: playhead
                    visible: tracksBackground.width > 0
                    x: 0
                    y: 0
                    width: 11
                    height: 5 
                } 

                MouseArea { 
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.SizeAllCursor
                    acceptedButtons: Qt.LeftButton
                    drag.target: cursorItem
                    drag.axis: Drag.XAxis
                    drag.minimumX: 0 

                    property bool mousePressed: false
                    onPressed: {
                        console.log("mouse press, x: ", cursorItem.x);
                        mousePressed = true;
                    }
                    onReleased: {
                        console.log("mouse release, x: ", cursorItem.x);
                        mousePressed = false;
                    }
                    onPositionChanged: {
                        if (mousePressed) {
                            console.log("mouse position changed: x", cursorItem.x);
                        } 
                    }
                }

                Connections {
                    target: TimelineWidget
                    onUpdateCursor: {
                        console.log("cursor step: ", TimelineModel.cursorStep);
                        cursorItem.x = cursorItem.x + TimelineModel.cursorStep;
                    }
                }
            }
             
        }
       
    }
    

    DelegateModel {
        id: tracksModel
        model: TimelineModel
        Track {
            model: TimelineModel
            rootIndex: tracksModel.modelIndex(index)
            color: (index == currentTrack)? sutdioYellow : selectedTrackColor;
            height: TimelineLogic.trackHeight(index == 1)
            width: TimelineModel.tracksAreaLength 
            isCurrentTrack: currentTrack == index 
            onClipClicked: { 
                currentTrack = track.DelegateModel.itemsIndex  
                TimelineWidget.selection = clip.DelegateModel.itemsIndex 
                track.selection = clip.DelegateModel.itemsIndex
                root.clipClicked() 
            }
            onClipDragged: {
                
            }
            onClipDropped: {
                
            } 
        } 
    }
    
}

