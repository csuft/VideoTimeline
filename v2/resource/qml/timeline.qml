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
    width: 10
    height: 10
    color: "transparent"

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
        height: 40
        anchors.top: parent.top
    }

    Row {
        anchors.top: toolbar.bottom
        MouseArea {
            id: tracksMouseArea
            width: root.width
            height: root.height - toolbar.height
            focus: true 
            hoverEnabled: true
            onClicked: {

            }
            onPositionChanged: {

            }

            Column {
                anchors.fill: parent
                Flickable {
                    contentX: tracksScrollView.flickableItem.contentX
                    width: root.width
                    height: ruler.height
                    interactive: false
                    Ruler {
                        id: ruler
                        width: tracksContainer.width 
                    }
                } 

                ScrollView {
                    id: tracksScrollView
                    width: root.width
                    height: root.height - ruler.heihgt - toolbar.height

                    Item {
                        id: tracksContainer
                        width: tracksScrollView.width + 300
                        height: tracksScrollView.height + 30 // padding

                    } 
                }

            }
        }
    }
}

