#include "thumbnailprovider.h"

#include <QQuickImageProvider>
#include <QCryptographicHash>   

namespace timeline {

	ThumbnailProvider::ThumbnailProvider()
		: QQuickImageProvider(QQmlImageProviderBase::Image, 
			QQmlImageProviderBase::ForceAsynchronousImageLoading) {

	}

	QImage ThumbnailProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
		QImage result;
		 
		
		return result;
	}

	QString ThumbnailProvider::cacheKey(const QString& service, const QString& resource, 
		const QString& hash, int frameNumber) { 
		QString key; 
		return key;
	}

	QImage ThumbnailProvider::makeThumbnail(int frameNumber, const QSize& requestedSize) {
		return QImage();
	}


}

