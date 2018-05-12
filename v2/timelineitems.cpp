#include "timelineitems.h" 

#include <QQuickPaintedItem>
#include <QPainter>
#include <QPalette>
#include <QPainterPath>
#include <QLinearGradient>

namespace timeline { 

	void TimelinePlayhead::paint(QPainter *painter) {
		QPainterPath path;
		path.moveTo(width(), 0);
		path.lineTo(width() / 2.0, height());
		path.lineTo(0, 0);
		QPalette p;
		painter->fillPath(path, p.color(QPalette::WindowText));
	}
	 
	TimelineTriangle::TimelineTriangle() {
		setAntialiasing(QPainter::Antialiasing);
	}

	void TimelineTriangle::paint(QPainter *painter) {
		QPainterPath path;
		path.moveTo(0, 0);
		path.lineTo(width(), 0);
		path.lineTo(0, height());
		painter->fillPath(path, Qt::black);
	} 

	TimelineWaveform::TimelineWaveform() {
		setAntialiasing(QPainter::Antialiasing);
		connect(this, SIGNAL(propertyChanged()), this, SLOT(update()));
	}

	void TimelineWaveform::paint(QPainter *painter) {
		QVariantList data = m_audioLevels.toList();
		if (data.isEmpty())
			return;

		// In and out points are # frames at current fps,
		// but audio levels are created at 25 fps.
		// Scale in and out point to 25 fps.
		const int inPoint = qRound(m_inPoint / 25.0);
		const int outPoint = qRound(m_outPoint / 25.0);
		const qreal indicesPrPixel = qreal(outPoint - inPoint) / width();

		QPainterPath path;
		path.moveTo(-1, height());
		int i = 0;
		for (; i < width(); ++i)
		{
			int idx = inPoint + int(i * indicesPrPixel);
			if (idx + 1 >= data.length())
				break;
			qreal level = qMax(data.at(idx).toReal(), data.at(idx + 1).toReal()) / 256;
			path.lineTo(i, height() - level * height());
		}
		path.lineTo(i, height());
		painter->fillPath(path, m_color.lighter());

		QPen pen(painter->pen());
		pen.setColor(m_color.darker());
		painter->strokePath(path, pen);
	} 

	void registerTimelineItems() {
		//qmlRegisterType<timeline::TimelinePlayhead>("Studio.Timeline", 1, 0, "TimelinePlayhead");
		qmlRegisterType<TimelineTriangle>("Studio.Timeline", 1, 0, "TimelineTriangle");
		qmlRegisterType<TimelineWaveform>("Studio.Timeline", 1, 0, "TimelineWaveform");
	}
}


