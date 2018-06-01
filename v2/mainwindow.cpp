#include "mainwindow.h" 
#include "qmlutilities.h"
#include "timelinetrackmodel.h"
#include "thumbnailprovider.h"
#include "qmlview.h"

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
		resize(810, 300);
#ifdef Q_OS_WIN 
		onVisibilityChanged(true);
#else
		connect(this, &QDockWidget::visibilityChanged, this, &MainWindow::load);
#endif  
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
		return QString::number(frames);
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
	
	void MainWindow::splitClip(int trackIndex) {
		qDebug() << "split clip " << trackIndex;
	}

	void MainWindow::removeClip(int trackIndex, int clipIndex) {
		qDebug() << "remove clip: track->" << trackIndex << " clip->" << clipIndex;
	}
}


