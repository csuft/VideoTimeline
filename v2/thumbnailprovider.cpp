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
		QString time = properties.frames_to_time(frameNumber, mlt_time_clock);
		// Reduce the precision to centiseconds to increase chance for cache hit
		// without much loss of accuracy.
		time = time.left(time.size() - 1);
		QString key;
		if (hash.isEmpty()) {
			key = QString("%1 %2 %3")
				.arg(service)
				.arg(resource)
				.arg(time);
			QCryptographicHash hash(QCryptographicHash::Sha1);
			hash.addData(key.toUtf8());
			key = hash.result().toHex();
		}
		else {
			key = QString("%1 %2").arg(hash).arg(time);
		}
		return key;
	}

	QImage ThumbnailProvider::makeThumbnail(int frameNumber, const QSize& requestedSize) {
		return QImage();
	}


}

