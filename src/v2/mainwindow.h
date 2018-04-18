#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H 

#include <QMainWindow>

namespace timeline {

	class MainWindow : public QMainWindow {
		Q_OBJECT
	public:
		MainWindow(QWidget* parent = Q_NULLPTR);
		~MainWindow() = default; 
	};
}

#endif // _MAIN_WINDOW_H