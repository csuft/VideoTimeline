#include "timelinetrackmodel.h" 
#include "mainwindow.h"    

#include <QScopedPointer>
#include <QApplication>
#include <qmath.h>
#include <QTimer> 

static const quintptr NO_PARENT_ID = quintptr(-1); 

namespace timeline {

	ClipInfo::ClipInfo(TrackIndex index) 
		: mTrackIndex(index) {

	}

	ClipInfo::~ClipInfo() {

	}

	TimelineTracksModel::TimelineTracksModel(QObject *parent)
		: QAbstractItemModel(parent) {
		connect(this, SIGNAL(modified()), SLOT(adjustBackgroundDuration()));
	}

	TimelineTracksModel::~TimelineTracksModel() {

	}

	// one video track and one audio track
	int TimelineTracksModel::rowCount(const QModelIndex &parent) const {
		Q_UNUSED(parent);
		return mTracks->count();
	}

	int TimelineTracksModel::columnCount(const QModelIndex &parent) const {
		Q_UNUSED(parent);
		return 1;
	}

	QVariant TimelineTracksModel::data(const QModelIndex &index, int role) const {
		if (!index.isValid())
		 	return QVariant();
		// Get info for a clip
		if (index.parent().isValid()) {
			TrackIndex trackIndex = (TrackIndex)index.internalId();
			int clipIndex = index.row();
			ClipInfo clipInfo = mTracks[trackIndex].at(clipIndex);
			// get clip info
			switch (role)
			{
			case NameRole:
				return clipInfo.getName(); 
			case SourceRole:
				return clipInfo.getSourcePath(); 
			case Qt::DisplayRole:
				return clipInfo.getName(); 
			case DurationRole:
				return clipInfo.getDuration(); 
			case InPointRole:
				return clipInfo.getInPoint(); 
			case OutPointRole:
				return clipInfo.getOutPoint();  
			case FrameRateRole:
				return clipInfo.getFrameRate();
			case AudioLevelsRole:
				return QVariant();
			default:
				break;
			} 
		}
		// Get info for a track
		else {
			TrackIndex trackIndex = (TrackIndex)index.row();
			switch (role)
			{
			case Qt::DisplayRole:
				return trackIndex == VideoTrack ? "Video Track" : "Audio Track"; 
			case DurationRole: {
				if (mTracks[trackIndex].size() > 0) {
					return mTracks[trackIndex].back().getOutPoint();
				}
				return 0;
			} 
			case IsAudioRole:
				return trackIndex == AudioTrack ? true : false; 
			default:
				break;
			} 
		}
		return QVariant();
	}

	QModelIndex TimelineTracksModel::index(int row, int column, const QModelIndex &parent) const {
		if (column > 0)
			return QModelIndex();

		QModelIndex result;
		if (parent.isValid()) { 
			// the internal identifier is track index
			result = createIndex(row, column, parent.row());
		}
		else if (row < tracksCount()) {
			result = createIndex(row, column, NO_PARENT_ID);
		}
		return result;
	}

	QModelIndex TimelineTracksModel::makeIndex(int trackIndex, int clipIndex) const {
		return index(clipIndex, 0, index(trackIndex));
	}

	QModelIndex TimelineTracksModel::parent(const QModelIndex &index) const { 
		if (!index.isValid() || index.internalId() == NO_PARENT_ID)
			return QModelIndex();
		else
			return createIndex(index.internalId(), 0, NO_PARENT_ID);
	}
	
	// exposed to QML
	QHash<int, QByteArray> TimelineTracksModel::roleNames() const {
		QHash<int, QByteArray> roles;
		roles[NameRole] = "name";
		roles[SourceRole] = "source";  
		roles[DurationRole] = "duration";
		roles[InPointRole] = "in";
		roles[OutPointRole] = "out"; 
		roles[IsAudioRole] = "audio";
		roles[FrameRateRole] = "fps";
		roles[AudioLevelsRole] = "audioLevels";   
		return roles;
	} 

	bool TimelineTracksModel::trimClipInValid(int trackIndex, int clipIndex, int delta) {
		return true;
	}

	int TimelineTracksModel::trimClipIn(int trackIndex, int clipIndex, int delta) { 
		return true;
	}

	void TimelineTracksModel::notifyClipIn(int trackIndex, int clipIndex) {
		if (trackIndex >= 0 && trackIndex < tracksCount() && clipIndex >= 0) {
			QModelIndex index = createIndex(clipIndex, 0, trackIndex);
			QVector<int> roles;
			roles << AudioLevelsRole;
			emit dataChanged(index, index, roles);
		} 
	}

	bool TimelineTracksModel::trimClipOutValid(int trackIndex, int clipIndex, int delta) {
		return true;
	}

	int TimelineTracksModel::trackHeight() const {
		return 50;
	}

	void TimelineTracksModel::setTrackHeight(int height) { 
		emit trackHeightChanged();
	}

	double TimelineTracksModel::scaleFactor() const {
		return 1.0;
	}

	void TimelineTracksModel::setScaleFactor(double scale) {  
		emit scaleFactorChanged();
	}

	int TimelineTracksModel::trimClipOut(int trackIndex, int clipIndex, int delta) {
		return 0;
	}

	void TimelineTracksModel::notifyClipOut(int trackIndex, int clipIndex) {
		if (trackIndex >= 0 && trackIndex < tracksCount() && clipIndex >= 0) {
			QModelIndex index = createIndex(clipIndex, 0, trackIndex);
			QVector<int> roles;
			roles << AudioLevelsRole;
			emit dataChanged(index, index, roles); 
		} 
	}

	bool TimelineTracksModel::moveClipValid(int clipIndex, int position) {
		return true;
	}

	bool TimelineTracksModel::moveClip(int clipIndex, int position) { 
		return true;
	} 

	int TimelineTracksModel::insertClip(int trackIndex, const ClipInfo& clip, int position) {
		// 
		return -1;
	}

	int TimelineTracksModel::appendClip(int trackIndex) {
		return -1;
	}

	void TimelineTracksModel::removeClip(int trackIndex, int clipIndex) {

	}

	void TimelineTracksModel::splitClip(int trackIndex, int clipIndex, int position) {
		
	}

	void TimelineTracksModel::joinClips(int trackIndex, int clipIndex) {
		
	}
	
	void TimelineTracksModel::moveClipToEnd(int trackIndex, int clipIndex, int position) {
		
	} 

	void TimelineTracksModel::audioLevelsReady(const QModelIndex& index) {
		QVector<int> roles;
		roles << AudioLevelsRole;
		emit dataChanged(index, index, roles);
	}

	void TimelineTracksModel::adjustBackgroundDuration() {
	
	}  

	void TimelineTracksModel::load() { 

	}

	void TimelineTracksModel::reload() { 
		beginResetModel();
		endResetModel();
		getAudioLevels();
	}

	void TimelineTracksModel::close() {
		emit closed();
	}

	int TimelineTracksModel::clipIndex(int trackIndex, int position) {
		
		return -1;
	}

	void TimelineTracksModel::getAudioLevels() {
		
	} 

}

