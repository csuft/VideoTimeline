#include "mainwindow.h" 
#include <QDebug>

namespace timeline {

	MainWindow::MainWindow(QWidget* parent /* = Q_NULLPTR */)
		: QMainWindow(parent)
	{
		setWindowIcon(QIcon(":/images/logo")); 
		resize(800, 150);
		
	}

}


