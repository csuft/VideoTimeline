#ifndef _AUDIO_LEVELS_TASK_H
#define _AUDIO_LEVELS_TASK_H
 
#include <QRunnable>
#include "timelinetrackmodel.h"

namespace timeline { 

	class AudioLevelsTask : public QRunnable {
	public:
		AudioLevelsTask(TimelineTracksModel* model, const QModelIndex& modelIndex);
		virtual ~AudioLevelsTask();

		static void start(TimelineTracksModel* model, const QModelIndex& modelIndex, bool force = false);
		static void stopAll();
		bool operator==(AudioLevelsTask& task);

	protected:
		virtual void run() override;

	private:
		QString cacheKey();

	private:
		TimelineTracksModel* mModel;
		bool mIsCanceled;
		bool mIsForce;
	};
}


#endif