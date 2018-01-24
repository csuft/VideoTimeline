#include "zoomer.h"

#include <QHBoxLayout>

namespace timeline {
	 
	Zoomer::Zoomer(QWidget* parent /* = Q_NULLPTR */)
		: QWidget(parent),
		mZoomIn(Q_NULLPTR),
		mZoomOut(Q_NULLPTR), 
		mZoomSlider(Q_NULLPTR)
	{
		setFixedWidth(150);
		
		initializeControls();
		QHBoxLayout* mainLayout = new QHBoxLayout(this);
		mainLayout->setSpacing(5);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(mZoomIn);
		mainLayout->addWidget(mZoomSlider);
		mainLayout->addWidget(mZoomOut); 
		setLayout(mainLayout); 
	}

	void Zoomer::initializeControls() {
		mZoomSlider = new QSlider(Qt::Horizontal, this);
		mZoomSlider->setRange(1, 6);
		mZoomSlider->setSliderPosition(3);
		
		mZoomIn = new QToolButton(this);
		mZoomIn->setIcon(QIcon(":/images/zoomin"));
		mZoomOut = new QToolButton(this);
		mZoomOut->setIcon(QIcon(":/images/zoomout")); 

		connect(mZoomSlider, &QAbstractSlider::valueChanged, this, &Zoomer::zoomerChanged);
		connect(mZoomIn, &QAbstractButton::clicked, this, &Zoomer::onZoomInClicked);
		connect(mZoomOut, &QAbstractButton::clicked, this, &Zoomer::onZoomOutClicked);
	}

	void Zoomer::onZoomInClicked(bool checked) {
		int currentValue = mZoomSlider->value();
		if (currentValue > 1) {
			mZoomSlider->setSliderPosition(--currentValue);
			emit zoomerChanged(currentValue);
		}
	}

	void Zoomer::onZoomOutClicked(bool checked) {
		int currentValue = mZoomSlider->value();
		if (currentValue < 6) {
			mZoomSlider->setSliderPosition(++currentValue);
			emit zoomerChanged(currentValue);
		}
	}
}