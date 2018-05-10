#ifndef MULTITRACKMODEL_H
#define MULTITRACKMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QString> 

namespace timeline {

	typedef enum {
		AudioTrackType = 0,
		VideoTrackType
	} TrackType; 

	class TimelineTracksModel : public QAbstractItemModel {
		Q_OBJECT
		Q_PROPERTY(int trackHeight READ trackHeight WRITE setTrackHeight NOTIFY trackHeightChanged)
		Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged) 
	public: 
		enum {
			NameRole = Qt::UserRole + 1,
			ResourceRole,   
			ServiceRole,    
			IsBlankRole,      
			StartRole,      
			DurationRole,
			InPointRole,     
			OutPointRole,    
			FramerateRole,   
			IsAudioRole,
			AudioLevelsRole, 
			IsTransitionRole, 
			FileHashRole,   
			IsLockedRole,  
			IsMuteRole,    
			IsHiddenRole,   
		};

		explicit TimelineTracksModel(QObject *parent = 0);
		~TimelineTracksModel(); 

		int rowCount(const QModelIndex &parent) const;
		int columnCount(const QModelIndex &parent) const;
		QVariant data(const QModelIndex &index, int role) const;
		QModelIndex index(int row, int column = 0,
			const QModelIndex &parent = QModelIndex()) const;
		QModelIndex makeIndex(int trackIndex, int clipIndex) const;
		QModelIndex parent(const QModelIndex &index) const;
		QHash<int, QByteArray> roleNames() const;
		void audioLevelsReady(const QModelIndex &index);
		bool createIfNeeded();
		void load();
		Q_INVOKABLE void reload();
		void close();
		int clipIndex(int trackIndex, int position);
		bool trimClipInValid(int trackIndex, int clipIndex, int delta, bool ripple);
		bool trimClipOutValid(int trackIndex, int clipIndex, int delta, bool ripple);
		int trackHeight() const;
		void setTrackHeight(int height);
		double scaleFactor() const;
		void setScaleFactor(double scale);
		void insertOrAdjustBlankAt(QList<int> tracks, int position, int length);
		bool mergeClipWithNext(int trackIndex, int clipIndex, bool dryrun);

	signals:
		void created();
		void loaded();
		void closed();
		void modified();
		void seeked(int position);
		void trackHeightChanged();
		void scaleFactorChanged();
		void showStatusMessage(const QString& message);
		void durationChanged(); 

	public slots: 
		void setTrackName(int row, const QString &value);
		void setTrackMute(int row, bool mute);
		void setTrackHidden(int row, bool hidden); 
		void setTrackLock(int row, bool lock);
		int trimClipIn(int trackIndex, int clipIndex, int delta, bool ripple);
		void notifyClipIn(int trackIndex, int clipIndex);
		int trimClipOut(int trackIndex, int clipIndex, int delta, bool ripple);
		void notifyClipOut(int trackIndex, int clipIndex);
		bool moveClipValid(int fromTrack, int toTrack, int clipIndex, int position);
		bool moveClip(int fromTrack, int toTrack, int clipIndex, int position);
		int overwriteClip(int trackIndex, int position, bool seek = true);
		QString overwrite(int trackIndex,int position, bool seek = true);
		int insertClip(int trackIndex, int position);
		int appendClip(int trackIndex);
		void removeClip(int trackIndex, int clipIndex); 
		void splitClip(int trackIndex, int clipIndex, int position);
		void joinClips(int trackIndex, int clipIndex); 
		void fadeIn(int trackIndex, int clipIndex, int duration);
		void fadeOut(int trackIndex, int clipIndex, int duration);
		bool addTransitionValid(int fromTrack, int toTrack, int clipIndex, int position);
		int addTransition(int trackIndex, int clipIndex, int position);
		void removeTransition(int trackIndex, int clipIndex);
		void removeTransitionByTrimIn(int trackIndex, int clipIndex, int delta);
		void removeTransitionByTrimOut(int trackIndex, int clipIndex, int delta);
		bool trimTransitionInValid(int trackIndex, int clipIndex, int delta);
		void trimTransitionIn(int trackIndex, int clipIndex, int delta);
		bool trimTransitionOutValid(int trackIndex, int clipIndex, int delta);
		void trimTransitionOut(int trackIndex, int clipIndex, int delta);
		bool addTransitionByTrimInValid(int trackIndex, int clipIndex, int delta);
		void addTransitionByTrimIn(int trackIndex, int clipIndex, int delta);
		bool addTransitionByTrimOutValid(int trackIndex, int clipIndex, int delta);
		void addTransitionByTrimOut(int trackIndex, int clipIndex, int delta);
		bool removeTransitionByTrimInValid(int trackIndex, int clipIndex, int delta);
		bool removeTransitionByTrimOutValid(int trackIndex, int clipIndex, int delta); 

	private: 
		bool m_isMakingTransition;

		void moveClipToEnd(int trackIndex, int clipIndex, int position);
		void relocateClip(int trackIndex, int clipIndex, int position);
		void moveClipInBlank(int trackIndex, int clipIndex, int position);
		void consolidateBlanks(int trackIndex);
		void consolidateBlanksAllTracks();
		void getAudioLevels();
		void removeBlankPlaceholder(int trackIndex);
		void removeRegion(int trackIndex, int position, int length);

	private slots:
		void adjustBackgroundDuration();
	}; 
}


#endif // MULTITRACKMODEL_H
