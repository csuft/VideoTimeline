function scrollIfNeeded() {
    var x = TimelineWidget.position * TimelineModel.scaleFactor;
    if (!tracksScrollView) return;
    if (x > tracksScrollView.flickableItem.contentX + tracksScrollView.width - 50)
        tracksScrollView.flickableItem.contentX = x - tracksScrollView.width + 50;
    else if (x < 50)
        tracksScrollView.flickableItem.contentX = 0;
    else if (x < tracksScrollView.flickableItem.contentX + 50)
        tracksScrollView.flickableItem.contentX = x - 50;
}

function trackHeight(isAudio) {
    return isAudio? Math.max(40, TimelineModel.trackHeight) : TimelineModel.trackHeight * 2
}
