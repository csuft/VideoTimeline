#ifndef TIMELINETRACKMODEL_H
#define TIMELINETRACKMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QString> 

namespace timeline { 

	typedef enum {
		AudioTrack = 0,
		VideoTrack
	} TrackIndex; 

	class TimelineTracksModel : public QAbstractItemModel {
		Q_OBJECT
		Q_PROPERTY(int trackHeight READ trackHeight WRITE setTrackHeight NOTIFY trackHeightChanged)
		Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged) 
	public: 
		enum { 
			NameRole = Qt::UserRole + 1,
			SourceRole,      
			IsBlankRole,      
			StartRole,      
			DurationRole,
			InPointRole,     
			OutPointRole,      
			IsAudioRole,
			AudioLevelsRole   
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
		bool trimClipInValid(int trackIndex, int clipIndex, int delta);
		bool trimClipOutValid(int trackIndex, int clipIndex, int delta);
		int trackHeight() const;
		void setTrackHeight(int height);
		double scaleFactor() const;
		void setScaleFactor(double scale);
		void insertOrAdjustBlankAt(QList<int> tracks, int position, int length); 

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
		int trimClipIn(int trackIndex, int clipIndex, int delta);
		void notifyClipIn(int trackIndex, int clipIndex);
		int trimClipOut(int trackIndex, int clipIndex, int delta);
		void notifyClipOut(int trackIndex, int clipIndex);
		bool moveClipValid(int clipIndex, int position);
		bool moveClip(int clipIndex, int position); 
		int insertClip(int trackIndex, int position);
		int appendClip(int trackIndex);
		void removeClip(int trackIndex, int clipIndex); 
		void splitClip(int trackIndex, int clipIndex, int position);
		void joinClips(int trackIndex, int clipIndex);  

	private:
		void moveClipToEnd(int trackIndex, int clipIndex, int position);
		void relocateClip(int trackIndex, int clipIndex, int position);
		void moveClipInBlank(int trackIndex, int clipIndex, int position);
		void consolidateBlanks(int trackIndex);
		void consolidateBlanksAllTracks();
		void getAudioLevels();
		void removeBlankPlaceholder(int trackIndex);
		int tracksCount() const { return 2; }

	private slots:
		void adjustBackgroundDuration();
	}; 
}


#endif // TIMELINETRACKMODEL_H
