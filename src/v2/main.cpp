#include <QApplication>
#include <QFile>
#include "mainwindow.h" 

int main(int argc, char* argv[]) {

	QApplication app(argc, argv); 

	timeline::MainWindow timeline;
	timeline.show();

	return app.exec();
}