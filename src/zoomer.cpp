#include "zoomer.h"

#include <QHBoxLayout>

namespace timeline {
	 
	Zoomer::Zoomer(QWidget* parent /* = Q_NULLPTR */)
		: QWidget(parent),
		mZoomIn(Q_NULLPTR),
		mZoomOut(Q_NULLPTR),
		mScrollBar(Q_NULLPTR),
		mZoomSlider(Q_NULLPTR)
	{
		setFixedHeight(20);
		mZoomSlider = new QSlider(Qt::Horizontal, this);
		mZoomIn = new QToolButton(this);
		mZoomIn->setIcon(QIcon(":/images/zoomin"));
		mZoomOut = new QToolButton(this);
		mZoomOut->setIcon(QIcon(":/images/zoomout"));
		mScrollBar = new QScrollBar(Qt::Horizontal, this);

		QHBoxLayout* mainLayout = new QHBoxLayout(this);
		mainLayout->setSpacing(5);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(mZoomIn);
		mainLayout->addWidget(mZoomSlider);
		mainLayout->addWidget(mZoomOut);
		mainLayout->addWidget(mScrollBar, 1);
		setLayout(mainLayout);
	}
}