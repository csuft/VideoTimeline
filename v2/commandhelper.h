#ifndef _COMMAND_HELPER_H
#define _COMMAND_HELPER_H

#include <QString>
#include <QMap>
#include <QList> 
#include <QUuid>

#include "timelinetrackmodel.h"

namespace timeline {

	class CommandHelper{  
	public:
		explicit CommandHelper(TimelineTracksModel& model);

		void recordBefore();
		void recordAfter();
		void undoChanges();
		void debugPrint();

	private:
		enum class ChangeFlags {
			NoChange = 0x0,
			ClipInfoModified = 0x1,
			ClipMoved = 0x2,
			ClipRemoved = 0x4
		};

		struct CommandState {
			int oldTrackIndex;  // drag & drop operations
			int oldClipIndex;
			int newTrackIndex;
			int newClipIndex;
			bool isBlank;
			int frameIn;   // trim operations
			int frameOut;   

			ChangeFlags changesType; 
			CommandState()
				: oldTrackIndex(-1),
				oldClipIndex(-1),
				newTrackIndex(-1),
				newClipIndex(-1),
				isBlank(false),
				frameIn(-1),
				frameOut(-1),
				changesType(ChangeFlags::NoChange) 
			{

			}
		};

		QMap<QUuid, CommandState> mStates;
		QList<QUuid> mClipsAddedOrder;
		QList<QUuid> mClipInsertedOrder;
		TimelineTracksModel& mModel;
	};
}

#endif // _COMMAND_HELPER_H
