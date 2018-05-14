import QtQuick 2.0
import QtQml.Models 2.1
import 'Track.js' as TrackLogic

Rectangle {
    id: trackRoot
    property alias model: trackModel.model
    property alias rootIndex: trackModel.rootIndex
    property bool isAudio
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
            timeline.remakeAudioLevels(trackRoot.DelegateModel.itemsIndex, i, force)
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
            isAudio: model.audio 
            audioLevels: model.audioLevels
            width: model.duration * timeScale
            height: trackRoot.height
            originalX: index * 50
            trackIndex: trackRoot.DelegateModel.itemsIndex  
            selected: trackRoot.isCurrentTrack

            onClicked: trackRoot.clipClicked(clip, trackRoot);
            onMoved: {}
            onDragged: {  }
            onTrimmingIn: { }
            onTrimmedIn: {}
            onTrimmingOut: {}
            onTrimmedOut: {} 
            onDropped: placeHolderAdded = false

            Component.onCompleted: {}
        }
    }

    Row {
        id: clipRow
        Repeater { id: repeater; model: trackModel }
    }
}
        //delegate: Rectangle {
        //     width: tracksBackground.width
        //     color: (index == currentTrack)? selectedTrackColor : sutdioYellow;
        //     height: TimelineLogic.trackHeight(false)
        //     Rectangle {
        //         id: clip
        //         color: (index == currentTrack) ? 'green' : 'black'
        //         width: 50
        //         height: parent.height
        //         x: 50 * index
        //         y: 0
        //         MouseArea {
        //             anchors.fill: parent
        //             acceptedButtons: Qt.LeftButton 
        //             drag.target: clip
        //             drag.axis: Drag.XAxis
        //             drag.minimumX: 0
        //             drag.maximumX: tracksBackground.width - clip.width
        //         }
        //     }
        // }
