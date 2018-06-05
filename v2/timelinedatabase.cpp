#include "timelinedatabase.h"

#include <QtSql>
#include <QSqlDatabase>

#include "mainwindow.h"

namespace timeline {

	struct DatabaseTask {
		enum Type {
			PutThumbnail,
			GetThumbnail
		}TransactionType;

		QImage image;
		QString hash;
		bool result;
		bool completed;
		DatabaseTask() : result(false), completed(false) {

		}
	};

	static TimelineDataBase* instance = Q_NULLPTR;

	TimelineDataBase::TimelineDataBase(QObject* parent)
		: QThread(parent),
		mCommitTimer(Q_NULLPTR) {

	}

	TimelineDataBase& TimelineDataBase::singleton(QWidget* parent) {
		if (!instance) {
			instance = new TimelineDataBase(parent);
			instance->start();
		}
		return *instance;
	}

	bool TimelineDataBase::putThumbnail(const QString& hash, const QImage& image) {
		DatabaseTask task;
		task.TransactionType = DatabaseTask::PutThumbnail;
		task.hash = hash;
		task.image = image;
		submitAndWait(&task);

		return task.result;
	}

	QImage TimelineDataBase::getThumbnail(const QString& hash) {
		DatabaseTask task;
		task.TransactionType = DatabaseTask::GetThumbnail;
		task.hash = hash;
		submitAndWait(&task);

		return task.image;
	}

	void TimelineDataBase::commitTransaction() {
		QSqlDatabase::database().commit();
	}

	void TimelineDataBase::shutDown() {
		requestInterruption();
		wait();
		QString connection = QSqlDatabase::database().connectionName();
		QSqlDatabase::database().close();
		QSqlDatabase::removeDatabase(connection);
		instance = Q_NULLPTR;
	}

	void TimelineDataBase::doTransaction(DatabaseTask* task) {
		if (!mCommitTimer->isActive()) {
			QSqlDatabase::database().transaction();
		}
		mCommitTimer->start();
		if (task->TransactionType == DatabaseTask::PutThumbnail) {
			QByteArray byteArray;
			QBuffer buffer(&byteArray);
			buffer.open(QIODevice::WriteOnly);
			task->image.save(&buffer, "PNG");

			QSqlQuery query;
			query.prepare("DELETE FROM thumbnails WHERE hash= :hash;");
			query.bindValue(":hash", task->hash);
			query.exec();
			query.prepare("INSERT INTO thumbnails VALUES(:hash, datetime('now'), :image);");
			query.bindValue(":hash", task->hash);
			query.bindValue(":image", byteArray);
			task->result = query.exec();
			if (!task->result) {
				qDebug() << query.lastError();
			}
		}
		else if (task->TransactionType == DatabaseTask::GetThumbnail) {
			QImage result;
			QSqlQuery query;
			query.prepare("SELECT image FROM thumbnails WHERE hash = :hash;");
			query.bindValue(":hash", task->hash);
			if (query.exec() && query.first()) {
				result.loadFromData(query.value(0).toByteArray(), "PNG");
				QSqlQuery update;
				update.prepare("UPDATE thumbnails SET accessed = datetime('now') WHERE hash = :hash;");
				update.bindValue(":hash", task->hash);
				task->result = update.exec();
				if (!task->result) {
					qDebug() << update.lastError();
				}
			}
			task->image = result;
		}
		deleteOldThumbnails();
		task->completed = true;
	}

	void TimelineDataBase::submitAndWait(DatabaseTask* task) {
		task->completed = false;
		mMutex.lock();
		mTasks.append(task);
		if (mTasks.size() == 1) {
			mWaitForNewTask.wakeAll();
		}
		while (!task->completed) {
			mWaitForFinished.wait(&mMutex);
		}
		mMutex.unlock();
	}

	void TimelineDataBase::deleteOldThumbnails() {
		QSqlQuery query;
		// delete the oldest 5000 records at most
		if (!query.exec("DELETE FROM thumbnails WHERE hash IN \
			(SELECT hash FROM thumbnails ORDER BY accessed DESC LIMIT -1 OFFSET 5000);")) {
			qDebug() << query.lastError();
		}
	}

	void TimelineDataBase::run() {
		connect(&MAINWINDOW, &MainWindow::aboutToShutdown, 
			this, &TimelineDataBase::shutDown, Qt::DirectConnection);
		QDir dir(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0]);
		if (!dir.exists()) {
			dir.mkpath(dir.path());
		}
		QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
		db.setDatabaseName(dir.filePath("db.sqlite3"));
		db.open();

		mCommitTimer = new QTimer();
		mCommitTimer->setSingleShot(true);
		mCommitTimer->setInterval(5000);
		connect(mCommitTimer, &QTimer::timeout, this, &TimelineDataBase::commitTransaction);
		 
		QSqlQuery query;
		bool success = query.exec("CREATE TABLE thumbnails (hash TEXT PRIMARY KEY NOT NULL, \
			accessed DATETIME NOT NULL, image BLOB);");
		if (!success) {
			qDebug() << "Failed to create thumbnails table.";
			return;
		} 

		while (true) {
			DatabaseTask* task = 0;
			mMutex.lock();
			if (mTasks.isEmpty()) {
				mWaitForNewTask.wait(&mMutex, 1000);
			}
			else {
				task = mTasks.takeFirst();
			}

			mMutex.unlock();
			QCoreApplication::processEvents();
			if (task) {
				doTransaction(task);
				mWaitForFinished.wakeAll();
			}
			if (isInterruptionRequested()) {
				break;
			}
		}

		if (mCommitTimer->isActive()) {
			commitTransaction();
		}
		delete mCommitTimer;
	}
}