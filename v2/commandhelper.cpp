#include "commandhelper.h"  

namespace timeline {

	CommandHelper::CommandHelper(TimelineTracksModel& model)
		: mModel(model){
	} 

	void CommandHelper::recordBefore() {
		mStates.clear();
		mClipsAddedOrder.clear();
		mClipInsertedOrder.clear();

		for (int track = 0; track < mModel.tracksCount(); ++track) {
			for (int clip = 0; clip < mModel.clipsCount(track); ++clip) {

			}

		}
	}

	void CommandHelper::recordAfter() {

	}

	void CommandHelper::undoChanges() {

	}

	void CommandHelper::debugPrint() {

	}
}

