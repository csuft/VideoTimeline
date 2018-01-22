#include <QApplication>
#include "ruler.h"

int main(int argc, char* argv[]) {

	QApplication app(argc, argv);

	timeline::TimelineWidget timeline;
	timeline.show();

	return app.exec();
}