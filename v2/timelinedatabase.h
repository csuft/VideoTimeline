#ifndef _TIMELINE_DATABASE_H
#define _TIMELINE_DATABASE_H 

#include <QWidget>
#include <QThread>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>

namespace timeline {

	struct DatabaseTask;

	class TimelineDataBase : public QThread {
		Q_OBJECT
			explicit TimelineDataBase(QObject* parent = Q_NULLPTR);
	public:
		static TimelineDataBase& singleton(QWidget* parent = Q_NULLPTR);
		bool putThumbnail(const QString& hash, const QImage& image);
		QImage getThumbnail(const QString& hash);

		private slots:
		void commitTransaction();
		void shutDown();

	private:
		void doTransaction(DatabaseTask* task);
		void submitAndWait(DatabaseTask* task);
		void deleteOldThumbnails();
		void run();

	private:
		QList<DatabaseTask*> mTasks;
		QMutex mMutex;
		QWaitCondition mWaitForFinished;
		QWaitCondition mWaitForNewTask;
		QTimer* mCommitTimer;
	};
}

#define DataBase timeline::TimelineDataBase::singleton()

#endif