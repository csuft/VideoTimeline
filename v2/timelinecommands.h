#ifndef _TIMELINE_COMMANDS_H
#define _TIMELINE_COMMANDS_H

#include <QUndoCommand>

#include "timelinetrackmodel.h"
#include "commandhelper.h"

namespace timeline { 

	// add, remove, copy, cut, split
	class AddCommand : public QUndoCommand {
		explicit AddCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
			int position, QUndoCommand* parent = Q_NULLPTR);
		virtual void redo();
		virtual void undo();

	private:
		TimelineTracksModel& mModel;
		
	};
	
	class RemoveCommand : public QUndoCommand {
		explicit RemoveCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
			int position, QUndoCommand* parent = Q_NULLPTR);
		virtual void redo();
		virtual void undo();

	private:
		TimelineTracksModel& mModel;
		
	};
	
	class CopyCommand : public QUndoCommand {
		explicit CopyCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
			int position, QUndoCommand* parent = Q_NULLPTR);
		virtual void redo();
		virtual void undo();

	private:
		TimelineTracksModel& mModel;
	};

	class CutCommand : public QUndoCommand {
		explicit CutCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
			int position, QUndoCommand* parent = Q_NULLPTR);
		virtual void redo();
		virtual void undo();

	private:
		TimelineTracksModel& mModel;
	};

	class SplitCommand : public QUndoCommand {
	public:
		explicit SplitCommand(TimelineTracksModel& model, int trackIndex, int clipIndex, 
			int position, QUndoCommand* parent = Q_NULLPTR);
		virtual void redo();
		virtual void undo();

	private:
		TimelineTracksModel& mModel;
		int mTrackIndex;
		int mClipIndex;
		int mPosition;
	};
}


#endif