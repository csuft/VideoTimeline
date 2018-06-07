#include <QApplication>
#include <QFile>
#include "mainwindow.h" 

int main(int argc, char* argv[]) {
	qSetMessagePattern("Message:%{message} File:%{file} Line:%{line} Function:%{function}");

	QApplication app(argc, argv); 
	app.setApplicationDisplayName("QmlBased Timeline");
	app.setApplicationName("QmlBased Timeline");
	app.setOrganizationDomain("com.hust");
	app.setOrganizationName("hust");

	timeline::MainWindow& mainWindow = MAINWINDOW;
	mainWindow.show();

	return app.exec();
}