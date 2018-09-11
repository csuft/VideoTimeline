#ifndef _TIMELINEDOCK_H
#define _TIMELINEDOCK_H

#include "timeline.h"
#include "timelinetrackmodel.h"
#include <QDockWidget>
#include <QDebug>

namespace timeline {

	class TimelineDock : public QWidget {
		Q_OBJECT
		Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
		Q_PROPERTY(int currentTrack READ currentTrack WRITE setCurrentTrack NOTIFY currentTrackChanged)
		Q_PROPERTY(int selection READ selection WRITE setSelection NOTIFY selectionChanged) 
	public:
		TimelineDock(QWidget *parent);
		~TimelineDock() = default; 

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
		void updateCursor();

	public slots:
		void onVisibilityChanged(bool visible);
		void addClip(int trackIndex);
		void copyClip(int trackIndex = -1);
		void cutClip(int trackIndex = -1, int clipIndex = -1);
		void splitClip(int trackIndex = -1, int clipIndex = -1);
		void removeClip(int trackIndex = -1, int clipIndex = -1); 

	private:
		void chooseClipAtPosition(int position, int& trackIndex, int& clipIndex);
		int clipIndexAtPosition(int trackIndex, int position);
		int clipIndexAtPlayhead(int trackIndex);
		bool getClipInfo(int trackIndex, int clipIndex, ClipInfo& clipInfo);

	private:
		TimelineTracksModel* mTimelineModel;
		Timeline* mTimelineWidget;
		int mPosition;
		int mCurrentTrack;
		int mSelection;
		int mVisibleTickStep;
	}; 
}  

#endif
