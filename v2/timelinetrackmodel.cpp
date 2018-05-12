#include "timelinetrackmodel.h" 
#include "mainwindow.h"    

#include <QScopedPointer>
#include <QApplication>
#include <qmath.h>
#include <QTimer> 

static const quintptr NO_PARENT_ID = quintptr(-1); 

namespace timeline {

	TimelineTracksModel::TimelineTracksModel(QObject *parent)
		: QAbstractItemModel(parent) {
		connect(this, SIGNAL(modified()), SLOT(adjustBackgroundDuration()));
	}

	TimelineTracksModel::~TimelineTracksModel() {

	}

	// one video track and one audio track
	int TimelineTracksModel::rowCount(const QModelIndex &parent) const {
		Q_UNUSED(parent);
		return tracksCount();
	}

	int TimelineTracksModel::columnCount(const QModelIndex &parent) const
	{
		Q_UNUSED(parent);
		return 1;
	}

	QVariant TimelineTracksModel::data(const QModelIndex &index, int role) const {
		if (!index.isValid())
		 	return QVariant();
		// Get info for a clip
		if (index.parent().isValid()) {
			TrackIndex type = (TrackIndex)index.internalId();
			// get clip info
			switch (role)
			{
			case NameRole:
				break;
			case SourceRole:
				break;
			case Qt::DisplayRole:
				break;
			case IsBlankRole:
				break;
			case StartRole:
				break;
			case DurationRole:
				break;
			case InPointRole:
				break;
			case OutPointRole:
				break;
			case AudioLevelsRole:
				break;
			default:
				break;
			} 
		}
		// Get info for a track
		else {
			TrackIndex trackType = (TrackIndex)index.row();
			switch (role)
			{
			case Qt::DisplayRole:

			case NameRole:
			case DurationRole:
			case IsAudioRole:

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
			TrackIndex trackType = (TrackIndex)parent.row();
			result = createIndex(row, column, parent.row());
		}
		// get index of track
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
		roles[IsBlankRole] = "blank";
		roles[StartRole] = "start";
		roles[DurationRole] = "duration";
		roles[InPointRole] = "in";
		roles[OutPointRole] = "out"; 
		roles[IsAudioRole] = "audio";
		roles[AudioLevelsRole] = "audioLevels";   
		return roles;
	}

	void TimelineTracksModel::setTrackName(int row, const QString &value) {
		if (row < tracksCount()) {  
			QModelIndex modelIndex = index(row, 0);
			QVector<int> roles;
			roles << NameRole;
			emit dataChanged(modelIndex, modelIndex, roles);
			emit modified();
		}
	}  

	bool TimelineTracksModel::trimClipInValid(int trackIndex, int clipIndex, int delta) {
		bool result = true; 
		return result;
	}

	int TimelineTracksModel::trimClipIn(int trackIndex, int clipIndex, int delta) {
		int result = clipIndex; 
		return result;
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
		bool result = true;
		return result;
	}

	int TimelineTracksModel::trackHeight() const {
		int result = 50;
		return result;
	}

	void TimelineTracksModel::setTrackHeight(int height) {
		emit trackHeightChanged();
	}

	double TimelineTracksModel::scaleFactor() const {
		double result = 0;
		return (result > 0) ? result : (qPow(1.0, 3.0) + 0.01);
	}

	void TimelineTracksModel::setScaleFactor(double scale) { 
		emit scaleFactorChanged();
	}

	int TimelineTracksModel::trimClipOut(int trackIndex, int clipIndex, int delta) {
		QList<int> tracksToRemoveRegionFrom;
		int result = clipIndex;
		int whereToRemoveRegion = -1;

		return result;
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
		bool result = false; 
		  
		return result;
	} 

	int TimelineTracksModel::insertClip(int trackIndex, int position) {
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

	void TimelineTracksModel::relocateClip(int trackIndex, int clipIndex, int position) {
		
	}

	void TimelineTracksModel::moveClipInBlank(int trackIndex, int clipIndex, int position) {
		
	}

	void TimelineTracksModel::consolidateBlanks(int trackIndex) {
		
	}

	void TimelineTracksModel::consolidateBlanksAllTracks() {

	}

	void TimelineTracksModel::audioLevelsReady(const QModelIndex& index) {
		QVector<int> roles;
		roles << AudioLevelsRole;
		emit dataChanged(index, index, roles);
	}

	bool TimelineTracksModel::createIfNeeded() { 
		return true;
	} 

	void TimelineTracksModel::adjustBackgroundDuration() {
	
	} 

	void TimelineTracksModel::insertOrAdjustBlankAt(QList<int> tracks, int position, int length) {
		
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

	void TimelineTracksModel::removeBlankPlaceholder(int trackIndex) { 

	}

}

