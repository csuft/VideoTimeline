import QtQuick 2.0
import QtQml.Models 2.1
import 'Track.js' as TrackLogic

Rectangle {
    id: trackRoot
    property alias model: trackModel.model
    property alias rootIndex: trackModel.rootIndex 
    property real timeScale: 1.0 
    property bool isCurrentTrack: false 
    property var selection

    signal clipClicked(var clip, var track)
    signal clipDragged(var clip, int x, int y)
    signal clipDropped(var clip) 
    signal checkSnap(var clip)

    function redrawWaveforms() {
        for (var i = 0; i < repeater.count; i++)
            repeater.itemAt(i).generateWaveform()
    }

    function remakeWaveforms(force) {
        for (var i = 0; i < repeater.count; i++)
            TimelineWidget.remakeAudioLevels(trackRoot.DelegateModel.itemsIndex, i, force)
    }

    function snapClip(clip) {
        TrackLogic.snapClip(clip, repeater)
    }

    function snapDrop(clip) {
        TrackLogic.snapDrop(clip, repeater)
    }

    function clipAt(index) {
        return repeater.itemAt(index)
    }

    color: 'transparent'
    width: clipRow.width

    DelegateModel {
        id: trackModel
        Clip {
            clipName: model.name
            clipSource: model.source
            clipDuration: model.duration 
            inPoint: model.in
            outPoint: model.out  
            audioLevels: model.audioLevels
            width: model.duration * timeScale
            height: trackRoot.height 
            trackIndex: trackRoot.DelegateModel.itemsIndex  
            selected: trackRoot.isCurrentTrack

            onClicked: trackRoot.clipClicked(clip, trackRoot);
            onMoved: {
                
            }
            onDragged: { 
                console.log("clip dragged: ", clip.x) 
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
                console.log("clip dropped： ", clip.x)
                trackRoot.clipDropped(clip)
            }
            Component.onCompleted: {

            }
        }
    }

    Row {
        id: clipRow
        Repeater { id: repeater; model: trackModel }
    }
} 
