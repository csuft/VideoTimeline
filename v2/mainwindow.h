#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H 

#include <QMainWindow>

#include "timeline.h"
#include "timelinetrackmodel.h"

namespace timeline {

	class MainWindow : public QMainWindow {
		Q_OBJECT 
		Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
		Q_PROPERTY(int currentTrack READ currentTrack WRITE setCurrentTrack NOTIFY currentTrackChanged)
		Q_PROPERTY(QList<int> selection READ selection WRITE setSelection NOTIFY selectionChanged)

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