#include "mainwindow.h" 
#include "qmlutilities.h"
#include "timelinetrackmodel.h"
#include "thumbnailprovider.h"
#include "qmlview.h"
#include "timelinedatabase.h"

#include <QApplication>
#include <QDebug>
#include <QVBoxLayout>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDockWidget>
#include <QQuickItem>

namespace timeline {

	MainWindow::MainWindow(QWidget* parent /* = Q_NULLPTR */)
		: QMainWindow(parent),
		mTimelineWidget(new Timeline(QmlUtilities::sharedEngine(), this)),
		mTimelineModel(new TimelineTracksModel),
		mPosition(0),
		mSelection(0) { 

		mUndoStack = new QUndoStack(this);
		QAction* undoAction = mUndoStack->createUndoAction(this);
		QAction* redoAction = mUndoStack->createRedoAction(this);
		undoAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0));
		redoAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Z", 0));

		mMenuBar = new QMenuBar(this);
		mMenuBar->addAction(undoAction);
		mMenuBar->addAction(redoAction); 

		TimelineDataBase::singleton(this);

		setWindowIcon(QIcon(":/images/images/audio-meter.png"));     
		qmlRegisterType<TimelineTracksModel>("Studio.Timeline", 1, 0, "TimelineTracksModel");
		QDir importPath = QmlUtilities::qmlDir();
		mTimelineWidget->engine()->addImportPath(importPath.path());
		mTimelineWidget->engine()->addImageProvider(QString("thumbnail"), new ThumbnailProvider);
		QmlUtilities::registerCommonTypes();
		QmlUtilities::setCommonProperties(mTimelineWidget->rootContext());
		mTimelineWidget->rootContext()->setContextProperty("view", new QmlView(mTimelineWidget));
		mTimelineWidget->rootContext()->setContextProperty("TimelineWidget", this);
		mTimelineWidget->rootContext()->setContextProperty("TimelineModel", mTimelineModel);
		mTimelineWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
		mTimelineWidget->setClearColor(palette().window().color());
		mTimelineWidget->setFocusPolicy(Qt::StrongFocus);
		mTimelineWidget->resize(800, 300); 
		setCentralWidget(mTimelineWidget);
		setMenuBar(mMenuBar);
		resize(810, 300);
#ifdef Q_OS_WIN 
		onVisibilityChanged(true);
#else
		connect(this, &QDockWidget::visibilityChanged, this, &MainWindow::load);
#endif  
	} 

	void MainWindow::closeEvent(QCloseEvent *event) {
		emit aboutToShutdown();
	}

	MainWindow& MainWindow::singleton() {
		static MainWindow* instance = new MainWindow;
		return *instance;
	}

	void MainWindow::onVisibilityChanged(bool visible) {
		if (visible) {
			load();
		}
	}

	void MainWindow::load(bool force /* = false */) {
		if (mTimelineWidget->source().isEmpty() || force) {
			QDir sourcePath = QmlUtilities::qmlDir();
			mTimelineWidget->setSource(QUrl("qrc:/script/qml/timeline.qml")); 
			if (force) {
				mTimelineModel->reload();
			}
		}
		else {
			mTimelineModel->reload();
		}
	}  

	void MainWindow::setPosition(int position) {
		if (position <= mTimelineModel->maxTrackLength()) {
			mPosition = position;
		}
		else {
			mPosition = mTimelineModel->maxTrackLength();
		}
		emit positionChanged();
	}

	void MainWindow::setCurrentTrack(int currentTrack) {
		if (currentTrack < 0 || currentTrack >=2) {
			mCurrentTrack = currentTrack;
		} 
	}
	
	int MainWindow::currentTrack() const {
		return mCurrentTrack;
	}

	void MainWindow::setSelection(int selection) {
		mSelection = selection; 
	}

	int MainWindow::selection() const {
		return mSelection;
	}

	QString MainWindow::timecode(int frames) {
		const int FFRAMERATE = 30.0;
		QString timestr("%1:%2:%3.%4");
		timestr = timestr.arg(frames / (FFRAMERATE*FFRAMERATE*FFRAMERATE))
			.arg(frames / (FFRAMERATE*FFRAMERATE))
			.arg(frames / (FFRAMERATE))
			.arg(frames % FFRAMERATE);
		return timestr;
	}

	void MainWindow::addClip(int trackIndex) {
		qDebug() << "add clip " << trackIndex;
	}

	void MainWindow::pasteClip(int trackIndex, int clipIndex) {
		qDebug() << "paste clip: track->" << trackIndex << " clip->" << clipIndex;
	}

	void MainWindow::copyClip(int trackIndex, int clipIndex) {
		qDebug() << "copy clip: track->" << trackIndex << " clip->" << clipIndex;
	}

	void MainWindow::cutClip(int trackIndex, int clipIndex) {
		qDebug() << "cut clip: track->" << trackIndex << " clip->" << clipIndex;
	}
	
	void MainWindow::splitClip(int trackIndex, int clipIndex) {
		qDebug() << "split clip: track->" << trackIndex << " position->" << mPosition;
		if (trackIndex < 0 || clipIndex < 0) {
			chooseClipAtPosition(mPosition, trackIndex, clipIndex);
		}
		if (trackIndex < 0 || clipIndex < 0) {
			qDebug() << "No available clip can be choosed";
			return;
		}
		setCurrentTrack(trackIndex);
		if (trackIndex >= 0 && clipIndex >= 0) {
			mTimelineModel->splitClip(trackIndex, clipIndex, mPosition);
		}
	}

	void MainWindow::removeClip(int trackIndex, int clipIndex) {
		qDebug() << "remove clip: track->" << trackIndex << " clip->" << clipIndex;
		if (trackIndex < 0 || clipIndex < 0) {
			return;
		}
		mTimelineModel->removeClip(trackIndex, clipIndex);
	}

	void MainWindow::chooseClipAtPosition(int position, int& trackIndex, int& clipIndex) {
		if (trackIndex >= 0) {
			clipIndex = clipIndexAtPosition(trackIndex, position);
			if (clipIndex >= 0 && !isBlankClip(trackIndex, clipIndex)) {
				return;
			}
		}
		
		trackIndex = currentTrack();
		clipIndex = clipIndexAtPosition(trackIndex, position);
		if (clipIndex >= 0 && !isBlankClip(trackIndex, clipIndex)) {
			return;
		}

		for (trackIndex = 0; trackIndex < mTimelineModel->tracksCount(); trackIndex++) {
			if (trackIndex == currentTrack()) {
				continue;
			}
			clipIndex = clipIndexAtPosition(trackIndex, position);
			if (clipIndex >= 0 && !isBlankClip(trackIndex, clipIndex)) {
				return;
			}
		}

		trackIndex = -1;
		clipIndex = -1;
	}

	bool MainWindow::isBlankClip(int trackIndex, int clipIndex) {
		Q_ASSERT(trackIndex >= 0 && clipIndex >= 0);
		return mTimelineModel->index(clipIndex, 0, mTimelineModel->index(trackIndex))
			.data(TimelineTracksModel::IsBlankRole).toBool();
	}

	int MainWindow::clipIndexAtPosition(int trackIndex, int position) {
		int clipIndex = -1;
		if (trackIndex < 0) {
			trackIndex = currentTrack();
		}
		if (trackIndex >= 0 && trackIndex < mTimelineModel->tracksCount()) {
			clipIndex = mTimelineModel->getClipIndexAt(trackIndex, position);
		}

		return clipIndex;
	}

	int MainWindow::clipIndexAtPlayhead(int trackIndex) {
		return clipIndexAtPosition(trackIndex, mPosition);
	}

	bool MainWindow::getClipInfo(int trackIndex, int clipIndex, ClipInfo& clipInfo) {
		if (trackIndex < 0 || trackIndex > mTimelineModel->tracksCount() 
			|| clipIndex < 0) {
			return false;
		}
		return mTimelineModel->getClipInfo(trackIndex, clipIndex, clipInfo);
	}
}
	

