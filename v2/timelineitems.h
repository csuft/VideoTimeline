#ifndef _TIMELINEITEMS_H
#define _TIMELINEITEMS_H

#include <QQuickPaintedItem>

namespace timeline {

	class TimelinePlayhead : public QQuickPaintedItem {
	public:
		TimelinePlayhead() {}
		~TimelinePlayhead() = default;
	protected:
		virtual void paint(QPainter *painter) override;
	};

	class TimelineTriangle : public QQuickPaintedItem {
	public:
		TimelineTriangle();
		~TimelineTriangle() = default;

	protected:
		virtual void paint(QPainter *painter) override;
	};

	class TimelineWaveform : public QQuickPaintedItem {
		Q_OBJECT
		Q_PROPERTY(QVariant levels MEMBER mAudioLevels NOTIFY propertyChanged)
		Q_PROPERTY(QColor fillColor MEMBER mColor NOTIFY propertyChanged)
		Q_PROPERTY(int inPoint MEMBER mInPoint NOTIFY inPointChanged)
		Q_PROPERTY(int outPoint MEMBER mOutPoint NOTIFY outPointChanged)

	public:
		TimelineWaveform();
		virtual void paint(QPainter *painter) override;

	signals:
		void propertyChanged();
		void inPointChanged();
		void outPointChanged();

	private:
		QVariant mAudioLevels;
		int mInPoint;
		int mOutPoint;
		QColor mColor;
	};

	void registerTimelineItems();
}  

#endif
