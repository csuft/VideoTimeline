#include "mainwindow.h" 
#include "timelinedatabase.h"

#include <QApplication>
#include <QTime>
#include <QVBoxLayout>
#include <QScrollArea>

namespace timeline {

	MainWindow::MainWindow(QWidget* parent /* = Q_NULLPTR */)
		: QMainWindow(parent) { 
		mUndoStack = new QUndoStack(this);
		QAction* undoAction = mUndoStack->createUndoAction(this);
		QAction* redoAction = mUndoStack->createRedoAction(this);
		undoAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0));
		redoAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Z", 0));
		setWindowIcon(QIcon(":/images/images/audio-meter.png"));
		mMenuBar = new QMenuBar(this);
		mMenuBar->addAction(undoAction);
		mMenuBar->addAction(redoAction); 
		setMenuBar(mMenuBar);

		TimelineDataBase::singleton(this); 
		mDockWidget = new TimelineDock(this);
		QScrollArea* area = new QScrollArea;
		area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		area->setWidget(mDockWidget);
		area->setWidgetResizable(true);
		setCentralWidget(area);

		resize(800, 300);
	} 

	void MainWindow::closeEvent(QCloseEvent *event) {
		emit aboutToShutdown();
	}

	MainWindow& MainWindow::singleton() {
		static MainWindow* instance = new MainWindow;
		return *instance;
	} 
}
	

