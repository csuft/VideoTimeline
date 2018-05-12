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
		Q_PROPERTY(QVariant levels MEMBER m_audioLevels NOTIFY propertyChanged)
		Q_PROPERTY(QColor fillColor MEMBER m_color NOTIFY propertyChanged)
		Q_PROPERTY(int inPoint MEMBER m_inPoint NOTIFY inPointChanged)
		Q_PROPERTY(int outPoint MEMBER m_outPoint NOTIFY outPointChanged)

	public:
		TimelineWaveform();
		virtual void paint(QPainter *painter) override;

	signals:
		void propertyChanged();
		void inPointChanged();
		void outPointChanged();

	private:
		QVariant m_audioLevels;
		int m_inPoint;
		int m_outPoint;
		QColor m_color;
	};

	void registerTimelineItems();
}  

#endif
