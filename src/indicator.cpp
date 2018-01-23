#include "indicator.h"
#include <QBitmap>
#include <QMouseEvent>

namespace timeline {

	Indicator::Indicator(QWidget* parent /* = Q_NULLPTR */)
		: QWidget(parent)
	{
		setMouseTracking(true);
		setCursor(Qt::SizeHorCursor);
		setAttribute(Qt::WA_TranslucentBackground, true);
		setMask(QBitmap(":/images/indicator"));
	}

	void Indicator::paintEvent(QPaintEvent *event) {
		return QWidget::paintEvent(event);
	}

	void Indicator::mouseMoveEvent(QMouseEvent *event) {
		if (event->buttons() & Qt::LeftButton)
		{
			emit indicatorMove(*event);
		} 
	}

	void Indicator::mousePressEvent(QMouseEvent *event) {
		if (event->button() == Qt::LeftButton)
		{
			emit indicatorPress(*event);
		}
	}

	void Indicator::mouseReleaseEvent(QMouseEvent *event) {
		if (event->button() == Qt::LeftButton)
		{
			emit indicatorRelease(*event);
		}
	} 
}