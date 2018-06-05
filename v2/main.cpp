#include <QApplication>
#include <QFile>
#include "mainwindow.h" 

int main(int argc, char* argv[]) {

	QApplication app(argc, argv); 

	timeline::MainWindow& mainWindow = MAINWINDOW;
	mainWindow.show();

	return app.exec();
}