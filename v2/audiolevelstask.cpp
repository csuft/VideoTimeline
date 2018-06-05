#include "audiolevelstask.h"

#include <QVariantList>
#include <QImage>
#include <QCryptographicHash>
#include <QRgb>
#include <QMutex>
#include <QThreadPool>

namespace timeline {
	
	AudioLevelsTask::AudioLevelsTask(TimelineTracksModel* model, const QModelIndex& modelIndex) {

	}
	AudioLevelsTask::~AudioLevelsTask() {

	}

	void AudioLevelsTask::start(TimelineTracksModel* model, const QModelIndex& modelIndex, bool force) {

	}
	void AudioLevelsTask::stopAll() {

	}

	bool AudioLevelsTask::operator==(AudioLevelsTask& task) {
		return true;
	}

	void AudioLevelsTask::run() {

	}

	QString AudioLevelsTask::cacheKey() {
		return QString();
	}
}