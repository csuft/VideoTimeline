#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H 

#include <QMainWindow>  
#include <QUndoStack> 
#include <QMenuBar>
 
#include "timelinedock.h"

namespace timeline {

	class MainWindow : public QMainWindow {
		Q_OBJECT  
	public:
		static MainWindow& singleton();
		~MainWindow() = default;  
		 
	signals:
		void aboutToShutdown();
	protected:
		void closeEvent(QCloseEvent *event) override;

	private:
		MainWindow(QWidget* parent = Q_NULLPTR);
		 
	private:
		TimelineDock* mDockWidget;
		QUndoStack* mUndoStack;
		QMenuBar* mMenuBar;
	};
}

#define MAINWINDOW timeline::MainWindow::singleton()

#endif // _MAIN_WINDOW_H