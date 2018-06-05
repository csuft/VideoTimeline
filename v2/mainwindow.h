#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H 

#include <QMainWindow> 
#include <QDebug>
#include <QUndoStack>
#include <QMenuBar>

#include "timeline.h"
#include "timelinetrackmodel.h"

namespace timeline {

	class MainWindow : public QMainWindow {
		Q_OBJECT 
		Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
		Q_PROPERTY(int currentTrack READ currentTrack WRITE setCurrentTrack NOTIFY currentTrackChanged) 
		Q_PROPERTY(int selection READ selection WRITE setSelection NOTIFY selectionChanged) 

	public:
		static MainWindow& singleton();
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
		void aboutToShutdown();
		void clipClicked();

	protected:
		void closeEvent(QCloseEvent *event) override;

	public slots:
		void onVisibilityChanged(bool visible);  
		void addClip(int trackIndex);
		void pasteClip(int trackIndex = -1, int clipIndex = -1);
		void copyClip(int trackIndex = -1, int clipIndex = -1);
		void cutClip(int trackIndex = -1, int clipIndex = -1);
		void splitClip(int trackIndex = -1, int clipIndex = -1);
		void removeClip(int trackIndex = -1, int clipIndex = -1);

	private:
		MainWindow(QWidget* parent = Q_NULLPTR);
		void chooseClipAtPosition(int position, int& trackIndex, int& clipIndex);
		int clipIndexAtPosition(int trackIndex, int position);
		bool isBlankClip(int trackIndex, int clipIndex);
		int clipIndexAtPlayhead(int trackIndex);
		bool getClipInfo(int trackIndex, int clipIndex, ClipInfo& clipInfo);

	private:
		Timeline* mTimelineWidget;
		TimelineTracksModel* mTimelineModel; 
		QUndoStack* mUndoStack;
		QMenuBar* mMenuBar;
		int mPosition;
		int mCurrentTrack; 
		int mSelection;
	};
}

#define MAINWINDOW timeline::MainWindow::singleton()

#endif // _MAIN_WINDOW_H