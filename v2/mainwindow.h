#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H 

#include <QMainWindow> 
#include <QDebug>

#include "timeline.h"
#include "timelinetrackmodel.h"

namespace timeline {

	class MainWindow : public QMainWindow {
		Q_OBJECT 
		Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
		Q_PROPERTY(int currentTrack READ currentTrack WRITE setCurrentTrack NOTIFY currentTrackChanged) 
		Q_PROPERTY(int selection READ selection WRITE setSelection NOTIFY selectionChanged) 

	public:
		MainWindow(QWidget* parent = Q_NULLPTR);
		~MainWindow() = default;  
		void load(bool force = false);
		int position() const { return mPosition; }
		void setPosition(int position);
		void setCurrentTrack(int currentTrack);
		int currentTrack() const; 
		void setSelection(int selection = 0);
		int selection() const;
		Q_INVOKABLE QString timecode(int frames);

	signals:
		void positionChanged();
		void currentTrackChanged();
		void selectionChanged(); 
		void clipClicked();

	public slots:
		void onVisibilityChanged(bool visible);  
		void addClip(int trackIndex);
		void pasteClip(int trackIndex = -1, int clipIndex = -1);
		void copyClip(int trackIndex = -1, int clipIndex = -1);
		void cutClip(int trackIndex = -1, int clipIndex = -1);
		void splitClip(int trackIndex = -1);
		void removeClip(int trackIndex = -1, int clipIndex = -1);

	private:
		Timeline* mTimelineWidget;
		TimelineTracksModel* mTimelineModel; 
		int mPosition;
		int mCurrentTrack; 
		int mSelection;
	};
}

#endif // _MAIN_WINDOW_H