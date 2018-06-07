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

	private:
		TrackIndex mTrackIndex;
		int mModelIndex;
		QString mName;
		QUrl mSourcePath;
		int mDuration;   // frame counts
		int mInPoint;    // frame index
		int mOutPoint;   // frame index
		qreal mFrameRate; 
	};
	Q_DECLARE_TYPEINFO(ClipInfo, Q_COMPLEX_TYPE);

	class TimelineTracksModel : public QAbstractItemModel {
		Q_OBJECT
		Q_PROPERTY(int trackHeight READ trackHeight WRITE setTrackHeight NOTIFY trackHeightChanged)
		Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged) 
		Q_PROPERTY(int maxTrackLength READ maxTrackLength NOTIFY maxTrackLengthChanged)
		Q_PROPERTY(int stepSize READ stepSize WRITE setStepSize NOTIFY stepSizeChanged)
	public: 
		enum { 
			NameRole = Qt::UserRole + 1,
			SourceRole,       
			DurationRole,
			InPointRole,     
			OutPointRole,      
			IsAudioRole, 
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
		int stepSize() const { return mStepSize; }
		void setStepSize(int stepSize);
		int tracksCount() const { return 2; }
		int clipsCount(int trackIndex);
		int getClipIndexAt(int trackIndex, int position);
		bool getClipInfo(int trackIndex, int clipIndex, ClipInfo& clipInfo);
		void resizeClip(int trackIndex, int clipIndex, int inPoint, int outPoint);
		bool insertClip(int trackIndex, int clipIndex, const ClipInfo& clip);

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
		void stepSizeChanged(int stepSize);

	public slots:  
		int trimClipIn(int trackIndex, int clipIndex, int delta);
		void notifyClipIn(int trackIndex, int clipIndex);
		int trimClipOut(int trackIndex, int clipIndex, int delta);
		void notifyClipOut(int trackIndex, int clipIndex);
		bool moveClipValid(int clipIndex, int position);
		bool moveClip(int clipIndex, int position);  
		int appendClip(int trackIndex);
		void removeClip(int trackIndex, int clipIndex); 
		void splitClip(int trackIndex, int clipIndex, int position);
		void joinClips(int trackIndex, int clipIndex);  

	private:
		void moveClipToEnd(int trackIndex, int clipIndex, int position); 
		void getAudioLevels(); 
		int randNumber(int low, int high); 

	private slots:
		void adjustBackgroundDuration();

	private: 
		QList<ClipInfo> mTracks[2];
		double mScaleFactor;
		int mTrackHeight;
		int mStepSize;
	}; 
}


#endif // TIMELINETRACKMODEL_H
