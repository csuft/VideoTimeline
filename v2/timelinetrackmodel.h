#ifndef TIMELINETRACKMODEL_H
#define TIMELINETRACKMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QUrl>
#include <QString> 

namespace timeline { 

	typedef enum {
		VideoTrack = 0,
		AudioTrack 
	} TrackIndex; 

	class ClipInfo{
	public:
		ClipInfo(TrackIndex index = VideoTrack);
		~ClipInfo();

		TrackIndex getTrackIndex() const { return mTrackIndex; }
		int getModelIndex() const { return mModelIndex; }
		void setModelIndex(int modelIndex) { mModelIndex = modelIndex; }
		QString getName() const { return mName; }
		void setName(const QString& name) { mName = name; }
		QUrl getSourcePath() const { return mSourcePath; }
		void setSourcePath(const QUrl& path) { mSourcePath = path; }
		int getDuration() const { return mDuration; }
		void setDuration(int duration) { mDuration = duration; }
		int getInPoint() const { return mInPoint; }
		void setInPoint(int in) { mInPoint = in; }
		int getOutPoint() const { return mOutPoint; }
		void setOutPoint(int out) { mOutPoint = out; }
		qreal getFrameRate() const { return mFrameRate; }
		void setFrameRate(qreal fps) { mFrameRate = fps; }
		bool isBlank() const { return mIsBlank; }
		void setBlank(bool blank) { mIsBlank = blank; }

	private:
		TrackIndex mTrackIndex;
		int mModelIndex;
		QString mName;
		QUrl mSourcePath;
		int mDuration;   // frame index
		int mInPoint;    // frame index
		int mOutPoint;   // frame index
		qreal mFrameRate;
		bool mIsBlank;
	};

	class TimelineTracksModel : public QAbstractItemModel {
		Q_OBJECT
		Q_PROPERTY(int trackHeight READ trackHeight WRITE setTrackHeight NOTIFY trackHeightChanged)
		Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged) 
		Q_PROPERTY(int maxTrackLength READ maxTrackLength NOTIFY maxTrackLengthChanged)
	public: 
		enum { 
			NameRole = Qt::UserRole + 1,
			SourceRole,       
			DurationRole,
			InPointRole,     
			OutPointRole,      
			IsAudioRole,
			IsBlankRole,
			FrameRateRole,
			AudioLevelsRole   
		};

		explicit TimelineTracksModel(QObject *parent = 0);
		~TimelineTracksModel(); 

		virtual int rowCount(const QModelIndex &parent) const override;
		virtual int columnCount(const QModelIndex &parent) const override;
		virtual QVariant data(const QModelIndex &index, int role) const override;
		virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
		virtual QModelIndex parent(const QModelIndex &index) const override;
		virtual QHash<int, QByteArray> roleNames() const override;

		QModelIndex makeIndex(int trackIndex, int clipIndex) const;
		void audioLevelsReady(const QModelIndex &index); 
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
		int maxTrackLength() const; 

	signals:
		void created();
		void loaded();
		void closed(); 
		void modified();
		void seeked(int position);
		void trackHeightChanged();
		void scaleFactorChanged(); 
		void durationChanged();
		void maxTrackLengthChanged();

	public slots:  
		int trimClipIn(int trackIndex, int clipIndex, int delta);
		void notifyClipIn(int trackIndex, int clipIndex);
		int trimClipOut(int trackIndex, int clipIndex, int delta);
		void notifyClipOut(int trackIndex, int clipIndex);
		bool moveClipValid(int clipIndex, int position);
		bool moveClip(int clipIndex, int position); 
		int insertClip(int trackIndex, const ClipInfo& clip, int position);
		int appendClip(int trackIndex);
		void removeClip(int trackIndex, int clipIndex); 
		void splitClip(int trackIndex, int clipIndex, int position);
		void joinClips(int trackIndex, int clipIndex);  

	private:
		void moveClipToEnd(int trackIndex, int clipIndex, int position); 
		void getAudioLevels(); 
		int tracksCount() const { return 2; }
		int randNumber(int low, int high);

	private slots:
		void adjustBackgroundDuration();

	private: 
		QList<ClipInfo> mTracks[2];
		double mScaleFactor;
		int mTrackHeight;
	}; 
}


#endif // TIMELINETRACKMODEL_H
