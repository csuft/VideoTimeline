import QtQuick 2.0
import QtQml.Models 2.1
import 'Track.js' as TrackLogic

Rectangle {
    id: trackRoot
    property alias model: trackModel.model
    property alias rootIndex: trackModel.rootIndex 
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

    color: 'transparent'
    width: clipRow.width

    DelegateModel {
        id: trackModel
        Clip {
            name: model.name
            source: model.source
            duration: model.duration 
            inPoint: model.in 
            outPoint: model.out
            audioLevels: model.audioLevels
            isAudio: model.audio 
            frameRate: model.fps
            width: model.duration / model.fps * timeScale * TimelineModel.stepSize
            height: trackRoot.height 
            trackIndex: trackRoot.DelegateModel.itemsIndex  
            selected: trackRoot.isCurrentTrack && trackRoot.selection == index
            onClicked: { 
                trackRoot.clipClicked(clip, trackRoot)  
            }
            onMoved: {  

            }
            onDragged: { 
                
            }
            onTrimmingIn: {

            }
            onTrimmedIn: {

            }
            onTrimmingOut: {

            }
            onTrimmedOut: {

            } 
            onDropped: {
                trackRoot.clipDropped(clip)
            }
        }
    }

    Row {
        id: clipRow
        Repeater { id: repeater; model: trackModel }
    }
} 
