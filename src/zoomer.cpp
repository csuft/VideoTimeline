#include "zoomer.h"

namespace timeline {
	 
	Zoomer::Zoomer(QWidget* parent /* = Q_NULLPTR */)
		: QWidget(parent),
		mZoomIn(Q_NULLPTR),
		mZoomOut(Q_NULLPTR),
		mScrollBar(Q_NULLPTR),
		mZoomSlider(Q_NULLPTR)
	{
		mZoomSlider = new QSlider(Qt::Horizontal, this);
		mZoomIn = new QToolButton();
	}
}