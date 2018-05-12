function scrollIfNeeded() {
    var x = timeline.position * timelinetracks.scaleFactor;
    if (!scrollView) return;
    if (x > scrollView.flickableItem.contentX + scrollView.width - 50)
        scrollView.flickableItem.contentX = x - scrollView.width + 50;
    else if (x < 50)
        scrollView.flickableItem.contentX = 0;
    else if (x < scrollView.flickableItem.contentX + 50)
        scrollView.flickableItem.contentX = x - 50;
}

function dragging(pos, duration) {
    if (tracksRepeater.count > 0) {
        var headerHeight = ruler.height + toolbar.height
        dropTarget.x = pos.x
        dropTarget.width = duration * timelinetracks.scaleFactor

        for (var i = 0; i < tracksRepeater.count; i++) {
            var trackY = tracksRepeater.itemAt(i).y + headerHeight - scrollView.flickableItem.contentY
            var trackH = tracksRepeater.itemAt(i).height
            if (pos.y >= trackY && pos.y < trackY + trackH) {
                currentTrack = i
                if (pos.x > headerWidth) {
                    dropTarget.height = trackH
                    dropTarget.y = trackY
                    if (dropTarget.y < headerHeight) {
                        dropTarget.height -= headerHeight - dropTarget.y
                        dropTarget.y = headerHeight
                    }
                    dropTarget.visible = true
                }
                break
            }
        }
        if (i === tracksRepeater.count || pos.x <= headerWidth)
            dropTarget.visible = false

        // Scroll tracks if at edges.
        if (pos.x > headerWidth + scrollView.width - 50) {
            // Right edge
            scrollTimer.backwards = false
            scrollTimer.start()
        } else if (pos.x >= headerWidth && pos.x < headerWidth + 50) {
            // Left edge
            if (scrollView.flickableItem.contentX < 50) {
                scrollTimer.stop()
                scrollView.flickableItem.contentX = 0;
            } else {
                scrollTimer.backwards = true
                scrollTimer.start()
            }
        } else {
            scrollTimer.stop()
        }

        if (toolbar.snap) {
            for (i = 0; i < tracksRepeater.count; i++)
                tracksRepeater.itemAt(i).snapDrop(pos)
        }
    }
}

function dropped() {
    dropTarget.visible = false
    scrollTimer.running = false
}

function acceptDrop(xml) {
    var position = Math.round((dropTarget.x + scrollView.flickableItem.contentX - headerWidth) / timelinetracks.scaleFactor)
    timeline.overwrite(currentTrack, position, xml)
}

function trackHeight(isAudio) {
    return isAudio? Math.max(40, timelinetracks.trackHeight) : timelinetracks.trackHeight * 2
}
