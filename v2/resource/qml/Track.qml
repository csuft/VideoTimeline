import QtQuick 2.0
import QtQml.Models 2.1 

ListView {
    id: trackRoot
    property alias dataModel: visualModel.model
    property alias rootIndex: visualModel.rootIndex 
    property real timeScale: 1.0 
    property bool isCurrentTrack: false 
    property int selection: 0
     
    signal clipClicked(var clip, var track)
    signal clipDragged(var clip, int x, int y)
    signal clipDropped(var clip)  

    function clipAt(index) {
        return repeater.itemAt(index)
    }

    function redrawAudioWaves() {
        for (var i = 0; i < repeater.count; ++i) {
            repeater.itemAt(i).generateWaves()
        }
    }

    function reloadAudioWaves(force) {
        for (var i = 0; i < repeater.count; ++i) {
            TimelineModel.reloadAudioWaves(trackRoot.DelegateModel.itemsIndex, i, force)
        }
    }

    Rectangle {
        color: isCurrentTrack ? 'black' : 'white'
        width: parent.width
        height: parent.height
        z: -1
    }
 
    width: parent.width
    orientation: ListView.Horizontal

    displaced: Transition {
        NumberAnimation { properties: "x,y"; easing.type: Easing.OutQuad }
    }
    model: DelegateModel {
        id: visualModel 
        delegate: MouseArea {
            id: delegateRoot

            property int visualIndex: DelegateModel.itemsIndex

            width: 100; height: trackRoot.height
            drag.target: icon

            Rectangle {
                id: icon
                width: 100; height: trackRoot.height
                anchors {
                    horizontalCenter: parent.horizontalCenter;
                    verticalCenter: parent.verticalCenter
                }
                color: 'red'
                radius: 3

                Drag.active: delegateRoot.drag.active
                Drag.source: delegateRoot
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
                    }
                ]
            }

            DropArea {
                anchors { fill: parent;}

                onEntered: { 
                    visualModel.items.move(drag.source.visualIndex, delegateRoot.visualIndex)
                }
            }
        }
    }

    // DelegateModel {
    //     id: trackModel 
    //     Clip {
    //         name: model.name
    //         source: model.source
    //         duration: model.duration 
    //         inPoint: model.in 
    //         outPoint: model.out
    //         audioLevels: model.audioLevels
    //         isAudio: model.audio 
    //         frameRate: model.fps
    //         x: model.in
    //         width: model.duration / model.fps * timeScale * TimelineModel.stepSize
    //         height: trackRoot.height 
    //         trackIndex: trackRoot.DelegateModel.itemsIndex  
            
    //         selected: trackRoot.isCurrentTrack && trackRoot.selection == index
    //         onClicked: { 
    //             trackRoot.clipClicked(clip, trackRoot)  
    //         }
    //         onMoved: {  

    //         }
    //         onDragged: { 
                
    //         }
    //         onTrimmingIn: {

    //         }
    //         onTrimmedIn: {

    //         }
    //         onTrimmingOut: {

    //         }
    //         onTrimmedOut: {

    //         } 
    //         onDropped: {
    //             trackRoot.clipDropped(clip)
    //         }
    //     }
    // } 
} 
