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
    // custom properties
    property int  currentTrack: 0
    property color selectedTrackColor: Qt.rgba(0.5, 0.5, 0, 0.3)
    property color sutdioYellow: Qt.rgba(255/255, 215/255, 0/255, 1.0)  
    
    // signals
    signal clipClicked()

    // signal handlers
    onCurrentTrackChanged: timeline.selection = []

    // functions

    // components
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
    }

    DropArea {
        anchors.fill: parent
        onEntered: {}
        onExited: {}
        onPositionChanged: {}
        onDropped: {}
    }

    TimelineToolbar {
        id: toolbar 
        width: parent.width
        height: 40
        anchors.top: parent.top
    }

    Row {
        anchors.top: toolbar.bottom

        MouseArea {
            id: tracksMouseArea
            anchors.fill: parent
            focus: true 
            hoverEnabled: true
            onClicked: {}
            onPositionChanged: {}

            Column {
                anchors.fill: parent
                Flickable {
                    contentX: tracksScrollView.flickableItem.contentX
                    width: tracksBackground.width
                    height: ruler.height
                    interactive: false
                    Ruler {
                        id: ruler
                        width: parent.width 
                    }
                } 

                ScrollView {
                    id: tracksScrollView
                    width: root.width
                    height: root.height - ruler.heihgt - toolbar.height

                    Item { 
                        width: tracksBackground.width
                        height: tracksScrollView.height + 30
                        
                        // tracks background
                        Column {
                            id: tracksBackground 
                            width: root.width + 100
                            height: parent.height
                            Repeater {
                                model: 2
                                delegate: Rectangle {
                                    width: tracksBackground.width
                                    color: (index == currentTrack)? selectedTrackColor : sutdioYellow;
                                    height: TimelineLogic.trackHeight(false)
                                    Rectangle {
                                        id: clip
                                        color: (index == currentTrack) ? 'green' : 'black'
                                        width: 50
                                        height: parent.height
                                        x: 250
                                        y: 0
                                        MouseArea {
                                            anchors.fill: parent
                                            drag.target: clip
                                            drag.axis: Drag.XAxis
                                            drag.minimumX: 0
                                            drag.maximumX: tracksBackground.width - clip.width
                                        }
                                    }
                                }
                            } 
                        }
                    } 
                } 
            }
            Rectangle {
                id: cursor
                visible: true
                color: 'black'
                width: 1
                height: tracksScrollView.height + 30 - tracksScrollView.__horizontalScrollBar.height
                x: 250
                y: 0
            }

            TimelinePlayhead {
                id: playhead
                visible: true
                x: cursor.x - 5
                y: 0
                width: 11
                height: 5
            }
        }
    }

    // DelegateModel {
    //     id: tracksDelegateModel
    //     model: 2
    //     Rectangle {
    //         width: parent.width
    //         color: 'transparent'
    //     }
    // }
}

