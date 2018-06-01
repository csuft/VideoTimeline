var SNAP_AREA = 10;

function snapClip(clip, repeater) {
    var left = clip.x;
    var right = clip.x + clip.width;
    if (clip.x > -SNAP_AREA && clip.x < SNAP_AREA) {
        // Snap around origin.
        clip.x = 0;
        return;
    } else {
        // Snap to other clips.
        for (i = 0; i < repeater.count; i+=1) {
            // Do not snap to self.
            if (i === clip.DelegateModel.itemsIndex && clip.trackIndex === repeater.itemAt(i).trackIndex)
                continue
            var itemLeft = repeater.itemAt(i).x
            var itemRight = itemLeft + repeater.itemAt(i).width
            // Snap to blank
            if (right > itemLeft - SNAP_AREA && right < itemLeft + SNAP_AREA) {
                // Snap right edge to left edge.
                clip.x = itemLeft - clip.width
                return
            } else if (left > itemRight - SNAP_AREA && left < itemRight + SNAP_AREA) {
                // Snap left edge to right edge.
                clip.x = itemRight
                return
            } else if (right > itemRight - SNAP_AREA && right < itemRight + SNAP_AREA) {
                // Snap right edge to right edge.
                clip.x = itemRight - clip.width
                return
            } else if (left > itemLeft - SNAP_AREA && left < itemLeft + SNAP_AREA) {
                // Snap left edge to left edge.
                clip.x = itemLeft
                return
            }
        }
    }

    var cursorX = tracksScrollView.flickableItem.contentX + cursor.x
    if (left > cursorX - SNAP_AREA && left < cursorX + SNAP_AREA)
        // Snap around cursor/playhead.
        clip.x = cursorX
    if (right > cursorX - SNAP_AREA && right < cursorX + SNAP_AREA)
        clip.x = cursorX - clip.width
} 

function snapDrop(pos, repeater) {
    var left = tracksScrollView.flickableItem.contentX + pos.x
    var right = left + dropTarget.width
    if (left > -SNAP_AREA && left < SNAP_AREA) {
        // Snap around origin.
        dropTarget.x = 0
        return
    } else {
        // Snap to other clips.
        for (var i = 0; i < repeater.count; i++) {
            var itemLeft = repeater.itemAt(i).x
            var itemRight = itemLeft + repeater.itemAt(i).width
            if (right > itemLeft - SNAP_AREA && right < itemLeft + SNAP_AREA) {
                dropTarget.x = itemLeft - dropTarget.width - tracksScrollView.flickableItem.contentX
                return
            } else if (left > itemRight - SNAP_AREA && left < itemRight + SNAP_AREA) {
                dropTarget.x = itemRight - tracksScrollView.flickableItem.contentX
                return
            }
        }
    }
    var cursorX = tracksScrollView.flickableItem.contentX + cursor.x
    if (left > cursorX - SNAP_AREA && left < cursorX + SNAP_AREA)
        // Snap around cursor/playhead.
        dropTarget.x = cursorX - tracksScrollView.flickableItem.contentX
    if (right > cursorX - SNAP_AREA && right < cursorX + SNAP_AREA)
        dropTarget.x = cursorX - dropTarget.width - tracksScrollView.flickableItem.contentX
}
