#include "timelinecommands.h"

namespace timeline {
	
	/// add clip command
	AddCommand::AddCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
		int position, QUndoCommand* parent)
		: mModel(model){

	} 

	void AddCommand::redo() {

	}

	void AddCommand::undo() {

	}
		 
	/// remove clip command
	RemoveCommand::RemoveCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
		int position, QUndoCommand* parent)
		: mModel(model) {

	}

	void RemoveCommand::redo() {

	}

	void RemoveCommand::undo() {

	}
		 
	/// copy clip command
	CopyCommand::CopyCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
		int position, QUndoCommand* parent)
		: mModel(model) {

	}

	void CopyCommand::redo() {

	}

	void CopyCommand::undo() {

	}
		 
	/// cut clip command
	CutCommand::CutCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
		int position, QUndoCommand* parent)
		: mModel(model) {

	}

	void CutCommand::redo() {

	}
	void CutCommand::undo() {

	}
		 
	/// split clip command
	SplitCommand::SplitCommand(TimelineTracksModel& model, int trackIndex, int clipIndex,
		int position, QUndoCommand* parent)
		: mModel(model) {

	}

	void SplitCommand::redo() {

	}

	void SplitCommand::undo() {

	}
}