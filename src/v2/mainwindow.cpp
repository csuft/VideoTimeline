#include "mainwindow.h" 
#include <QDebug>
#include <QVBoxLayout>

namespace timeline {

	MainWindow::MainWindow(QWidget* parent /* = Q_NULLPTR */)
		: QMainWindow(parent) {
		setWindowIcon(QIcon(":/images/logo")); 
		resize(800, 150); 
		mTimeline = new Timeline(this); 
		setCentralWidget(mTimeline);
	} 
}


