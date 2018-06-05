#include "audiolevelstask.h"

#include <QVariantList>
#include <QImage>
#include <QCryptographicHash>
#include <QRgb>
#include <QMutex>
#include <QDebug>
#include <QTime>
#include <QThreadPool>

#include "timelinedatabase.h"

namespace timeline {

	static QList<AudioLevelsTask*> tasksList;
	static QMutex tasksListMutex;
	
	AudioLevelsTask::AudioLevelsTask(TimelineTracksModel* model, 
		const QModelIndex& modelIndex)
		: QRunnable(),
		mModel(model),
		mIsCanceled(false),
		mIsForce(false),
		mModelIndex(modelIndex) {

	}

	AudioLevelsTask::~AudioLevelsTask() {

	}

	void AudioLevelsTask::start(TimelineTracksModel* model, 
		const QModelIndex& modelIndex, bool force) {
		if (modelIndex.isValid() && model != Q_NULLPTR) {
			AudioLevelsTask* task = new AudioLevelsTask(model, modelIndex);
			tasksListMutex.lock();
			foreach(AudioLevelsTask* t, tasksList) {
				if (*t == *task) {
					delete task;
					task = Q_NULLPTR;
					break;
				}
			}
			if (task) {
				task->mIsForce = force;
				tasksList << task;
				QThreadPool::globalInstance()->start(task);
			}
			tasksListMutex.unlock();
		}
	}

	void AudioLevelsTask::stopAll() {
		tasksListMutex.lock();
		while (!tasksList.isEmpty()) {
			AudioLevelsTask* task = tasksList.first();
			task->mIsCanceled = true;
			tasksList.removeFirst();
		}
		tasksListMutex.unlock();
	}

	bool AudioLevelsTask::operator==(AudioLevelsTask& task) {
		if (mModelIndex.internalId() == task.mModelIndex.internalId()
			&& mModelIndex.row() == task.mModelIndex.row()) {
			return true;
		}
		return false;
	}

	void AudioLevelsTask::run() {
		QVariantList levels;
		int channels = 2;

		QImage image = DataBase.getThumbnail(cacheKey());
		if (image.isNull() || mIsForce) {
			QTime updateTime;
			updateTime.start();
			
			// generate random audio levels
			for (int i = 0; i < 400; i++) {
				// fill data

				// incrementally update the audio levels every 5 seconds
				if (updateTime.elapsed() > 5000 && !mIsCanceled) {
					updateTime.restart();
					mModel->audioLevelsReady(mModelIndex);
				}
			}
			 
			if (!mIsCanceled) { 
				int count = levels.size();
				QImage image((count + 3) / 4 / channels, channels, QImage::Format_ARGB32);
				int n = image.width() * image.height();
				for (int i = 0; i < n; i++) {
					QRgb pixel;
					if ((4*i + 3) < count) {
						pixel = qRgba(levels.at(4 * i).toInt(), levels.at(4*i+1).toInt(),
							levels.at(4*i+2).toInt(), levels.at(4*i+3).toInt());
					}
					else {
						int last = levels.last().toInt();
						int r = (4 * i + 0) < count ? levels.at(4 * i + 0).toInt() : last;
						int g = (4 * i + 1) < count ? levels.at(4 * i + 1).toInt() : last;
						int b = (4 * i + 2) < count ? levels.at(4 * i + 2).toInt() : last;
						int a = last;
						pixel = qRgba(r, g, b, a);
					}
					image.setPixel(i / channels, i%channels, pixel);
				}
				if (!image.isNull()) {
					DataBase.putThumbnail(cacheKey(), image);
				}
				else {
					QImage image(1, 1, QImage::Format_ARGB32);
					DataBase.putThumbnail(cacheKey(), image);
				}
			}
		}
		else if (!mIsCanceled) {
			// convert cached image 
			int n = image.width() * image.height();
			for (int i = 0; n > 1 && i < n; i++) {
				QRgb p = image.pixel(i / channels, i % channels);
				levels << qRed(p);
				levels << qGreen(p);
				levels << qBlue(p);
				levels << qAlpha(p);
			}
		}

		tasksListMutex.lock();
		for (int i = 0; i < tasksList.size(); ++i) {
			if (*tasksList[i] == *this) {
				tasksList.removeAt(i);
				break;
			}
		}
		tasksListMutex.unlock();

		if (levels.size() > 0 && !mIsCanceled) {
			mModel->audioLevelsReady(mModelIndex);
		}
	}

	QString AudioLevelsTask::cacheKey() {
		QString key("%1 %2 audiolevels");
		key = key.arg(mModelIndex.internalId()).arg(mModelIndex.row());
		qDebug() << "cache key: " << key;

		QCryptographicHash hash(QCryptographicHash::Sha1);
		hash.addData(key.toUtf8());
		return hash.result().toHex();
	}
}