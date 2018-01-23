#include <QApplication>
#include <QFile>
#include "ruler.h"

int main(int argc, char* argv[]) {

	QApplication app(argc, argv);

	QFile file(":/style/app");
	bool ret = file.open(QFile::ReadOnly);
	if (ret) {
		qApp->setStyleSheet(file.readAll());
		file.close(); 
	}

	timeline::TimelineWidget timeline;
	timeline.show();

	return app.exec();
}