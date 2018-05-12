#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H 

#include <QMainWindow>
#include <QPushButton>

#include "timeline.h"
#include "timelinetrackmodel.h"

namespace timeline {

	class MainWindow : public QMainWindow {
		Q_OBJECT 
		Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
		Q_PROPERTY(int currentTrack READ currentTrack WRITE setCurrentTrack NOTIFY currentTrackChanged) 

	public:
		MainWindow(QWidget* parent = Q_NULLPTR);
		~MainWindow() = default;  
		void load(bool force = false);
		int position() const { return m_position; }
		void setPosition(int position);
		void setCurrentTrack(int currentTrack);
		int currentTrack() const; 

	signals:
		void positionChanged();
		void currentTrackChanged();
		void selectionChanged();

	private slots:
		void onVisibilityChanged(bool visible);
		void clearSelectionIfInvalid();
		void onAddClip();
	
	private:
		Timeline* mTimelineWidget;
		TimelineTracksModel* mTimelineModel;
		QPushButton* mAddClipBtn;
		int m_position;
	};
}

#endif // _MAIN_WINDOW_H