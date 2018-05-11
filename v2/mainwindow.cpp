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

namespace timeline {

	MainWindow::MainWindow(QWidget* parent /* = Q_NULLPTR */)
		: QMainWindow(parent),
		mTimelineWidget(new Timeline(QmlUtilities::sharedEngine(), this)),
		mTimelineModel(new TimelineTracksModel),
		mAddClipBtn(new QPushButton("Add Clip", this)){
		setWindowIcon(QIcon(":/images/logo")); 
		resize(800, 150);  

		QVBoxLayout* mainLayout = new QVBoxLayout;
		qmlRegisterType<TimelineTracksModel>("Studio.Timeline", 1, 0, "TimelineTracksModel");
		QDir importPath = QmlUtilities::qmlDir();
		mTimelineWidget->engine()->addImportPath(importPath.path());
		mTimelineWidget->engine()->addImageProvider(QString("thumbnail"), new ThumbnailProvider);
		QmlUtilities::registerCommonTypes();
		QmlUtilities::setCommonProperties(mTimelineWidget->rootContext());
		mTimelineWidget->rootContext()->setContextProperty("view", new QmlView(mTimelineWidget));
		mTimelineWidget->rootContext()->setContextProperty("timeline", this);
		mTimelineWidget->rootContext()->setContextProperty("timelinetracks", mTimelineModel);
		mTimelineWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
		mTimelineWidget->setClearColor(palette().window().color());
		mTimelineWidget->setFocusPolicy(Qt::StrongFocus);
		
		mainLayout->addWidget(mTimelineWidget, 1);
		mainLayout->addWidget(mAddClipBtn, 0, Qt::AlignRight);
		centralWidget()->setLayout(mainLayout);

#ifdef Q_OS_WIN 
		onVisibilityChanged(true);
#else
		connect(this, &QDockWidget::visibilityChanged, this, &MainWindow::load);
#endif
		connect(mTimelineModel, &TimelineTracksModel::modified, this, &MainWindow::clearSelectionIfInvalid);
		connect(mAddClipBtn, &QAbstractButton::clicked, this, &MainWindow::onAddClip);
	} 

	void MainWindow::onVisibilityChanged(bool visible) {
		if (visible) {
			load();
		}
	}

	void MainWindow::load(bool force /* = false */) {
		if (mTimelineWidget->source().isEmpty() || force) {
			QDir sourcePath = QmlUtilities::qmlDir();
			mTimelineWidget->setSource(QUrl::fromLocalFile(sourcePath.filePath("timeline.qml")));
			//disconnect(this, &QDockWidget::visibilityChanged, this, &MainWindow::onVisibilityChanged);
			//connect(mTimelineWidget->rootObject(), SIGNAL(currentTrackChanged), 
			//	this, SIGNAL(currentTrackChanged));
			//connect(mTimelineWidget->rootObject(), SIGNAL(clipClicked),
			//	this, SIGNAL(clipClicked));
			if (force) {
				// load audio wave data
				mTimelineModel->reload();
			}
		}
		else {
			mTimelineModel->reload();
		}
	}

	void MainWindow::clearSelectionIfInvalid() {

	}

	void MainWindow::onAddClip() {

	}
}


