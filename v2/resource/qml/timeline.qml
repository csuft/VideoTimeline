import QtQuick 2.2
import QtQml.Models 2.1
import QtQuick.Controls 1.0
import Studio.Timeline 1.0
import QtGraphicalEffects 1.0
import QtQuick.Window 2.2
import 'Timeline.js' as TimelineLogic

Rectangle {
    // system attributes
    id: root  
    color: 'transparent' 

    // functions 
    function setZoom(value) {
        if (value >= toolbar.minimum && value <= toolbar.maximum) {
            toolbar.scaleSlider.value = value;
            for (var i = 0; i < tracksRepeater.count; ++i) {
                tracksRepeater.itemAt(i).redrawAudioWaves();
            }
        } 
    }

    function adjustZoom(by) {
        setZoom(toolbar.scaleSlider.value + by)
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
    readonly property int padding: 30
    
    // signals
    signal clipClicked()

    // signal handlers
    onCurrentTrackChanged: {
        console.log("Current active track: ", currentTrack)
    }  

    TimelineToolbar {
        id: toolbar 
        width: parent.width
        height: 40
        anchors.top: parent.top
    }

    Row {
        anchors.top: toolbar.bottom  
        Column { 
            Flickable {
                contentX: tracksScrollView.flickableItem.contentX
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

            ScrollView {
                id: tracksScrollView
                width: root.width
                height: root.height - ruler.height - toolbar.height 
                
                Item { 
                    width: tracksBackground.width + padding
                    height: tracksScrollView.height
                    
                    // tracks
                    Column {
                        id: tracksBackground 
                        Repeater {
                            id: tracksRepeater
                            model: tracksModel 
                        } 
                    }
                } 
            } 
        }
        Rectangle {
            id: cursor
            visible: TimelineWidget.position > -1
            color: 'black'
            width: 1
            height: tracksScrollView.height + padding - tracksScrollView.__horizontalScrollBar.height
            x: TimelineWidget.position - tracksScrollView.flickableItem.contentX
            y: 0
        }

        TimelinePlayhead {
            id: playhead
            visible: TimelineWidget.position > -1
            x: TimelineWidget.position - tracksScrollView.flickableItem.contentX - 5
            y: 0
            width: 11
            height: 5
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

