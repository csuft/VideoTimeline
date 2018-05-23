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
		Q_PROPERTY(QList<int> selection READ selection WRITE setSelection NOTIFY selectionChanged)

	public:
		MainWindow(QWidget* parent = Q_NULLPTR);
		~MainWindow() = default;  
		void load(bool force = false);
		int position() const { qDebug() << "get pos: " << mPosition; return mPosition; }
		void setPosition(int position);
		void setCurrentTrack(int currentTrack);
		int currentTrack() const; 
		void setSelection(QList<int> selection = QList<int>(), int trackIndex = -1);
		QList<int> selection() const;
		Q_INVOKABLE QString timecode(int frames);

	signals:
		void positionChanged();
		void currentTrackChanged();
		void selectionChanged(); 

	private slots:
		void onVisibilityChanged(bool visible); 
		Q_INVOKABLE void onAddClip();
	
	private:
		Timeline* mTimelineWidget;
		TimelineTracksModel* mTimelineModel; 
		int mPosition;
		struct Selection {
			QList<int> selectedClips;
			int selectedTrack;
			bool isMultitrackSelected;
		};
		Selection mSelection;
	};
}

#endif // _MAIN_WINDOW_H