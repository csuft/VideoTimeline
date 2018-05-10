#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H 

#include <QMainWindow>

#include "timeline.h"
#include "timelinetrackmodel.h"

namespace timeline {

	class MainWindow : public QMainWindow {
		Q_OBJECT
	public:
		MainWindow(QWidget* parent = Q_NULLPTR);
		~MainWindow() = default;  
		void load(bool force = false);

	private slots:
		void onVisibilityChanged(bool visible);
		void clearSelectionIfInvalid();
	
	private:
		Timeline* mTimelineWidget;
		TimelineTracksModel* mTimelineModel;
	};
}

#endif // _MAIN_WINDOW_H