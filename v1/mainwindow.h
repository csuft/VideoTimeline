#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include <QScrollArea>
#include "ruler.h"
#include "zoomer.h" 

namespace timeline {

	class MainWindow : public QScrollArea {
		Q_OBJECT
	public:
		MainWindow(QWidget* parent = Q_NULLPTR);
		~MainWindow() = default;

	private:
		Ruler* mRuler;
		Zoomer* mZoomer;
	};
}

#endif // _MAIN_WINDOW_H