#include "timelinecommands.h"

namespace timeline {
	
	AddCommand::AddCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
		int position, QUndoCommand* parent)
		: mModel(model){

	} 

	void AddCommand::redo() {

	}

	void AddCommand::undo() {

	}
		 
	 
	RemoveCommand::RemoveCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
		int position, QUndoCommand* parent = Q_NULLPTR)
		: mModel(model) {

	}

	void RemoveCommand::redo() {

	}

	void RemoveCommand::undo() {

	}
		 
	CopyCommand::CopyCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
		int position, QUndoCommand* parent = Q_NULLPTR)
		: mModel(model) {

	}

	void CopyCommand::redo() {

	}

	void CopyCommand::undo() {

	}
		 
		 
	CutCommand::CutCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
		int position, QUndoCommand* parent = Q_NULLPTR)
		: mModel(model) {

	}

	void CutCommand::redo() {

	}
	void CutCommand::undo() {

	}
		 
		 
	SplitCommand::SplitCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
		int position, QUndoCommand* parent = Q_NULLPTR)
		: mModel(model) {

	}

	void SplitCommand::redo() {

	}

	void SplitCommand::undo() {

	}
}