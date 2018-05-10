#ifndef THUMBNAILPROVIDER_H
#define THUMBNAILPROVIDER_H

#include <QQuickImageProvider> 

namespace timeline {

	class ThumbnailProvider : public QQuickImageProvider {
	public:
		explicit ThumbnailProvider();
		QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

	private:
		QString cacheKey(const QString& service, const QString& resource, 
			const QString& hash, int frameNumber);
		QImage makeThumbnail(int frameNumber, const QSize& requestedSize); 
	}; 
} 

#endif // THUMBNAILPROVIDER_H
