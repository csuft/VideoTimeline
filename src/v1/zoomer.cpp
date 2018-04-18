#include "zoomer.h"

#include <QHBoxLayout>

namespace timeline {
	 
	Zoomer::Zoomer(QWidget* parent /* = Q_NULLPTR */)
		: QWidget(parent),
		mZoomIn(Q_NULLPTR),
		mZoomOut(Q_NULLPTR),
		mZoomSlider(Q_NULLPTR),
		mCurrentLevel(1)
	{
		setFixedWidth(150);
		setCursor(Qt::PointingHandCursor);
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
		mZoomSlider->setRange(MIN_LEVEL, MAX_LEVEL); // every step is 2.5 
		mZoomSlider->setSliderPosition(MIN_LEVEL);
		
		mZoomIn = new QToolButton(this);
		mZoomIn->setIcon(QIcon(":/images/zoomin"));
		mZoomOut = new QToolButton(this);
		mZoomOut->setIcon(QIcon(":/images/zoomout")); 

		connect(mZoomSlider, &QAbstractSlider::valueChanged, this, &Zoomer::onSliderChanged);
		connect(mZoomIn, &QAbstractButton::clicked, this, &Zoomer::onZoomInClicked);
		connect(mZoomOut, &QAbstractButton::clicked, this, &Zoomer::onZoomOutClicked);
	}

	void Zoomer::onZoomInClicked(bool checked) {
		int currentValue = mZoomSlider->value();
		if (currentValue > MIN_LEVEL) {
			mZoomSlider->setSliderPosition(--currentValue);
			emit zoomerIn(currentValue);
		}
	}

	void Zoomer::onZoomOutClicked(bool checked) {
		int currentValue = mZoomSlider->value();
		if (currentValue < MAX_LEVEL) {
			mZoomSlider->setSliderPosition(++currentValue);
			emit zoomerOut(currentValue);
		}
	}

	void Zoomer::onSliderChanged(int value) {
		if (value > mCurrentLevel) {
			emit zoomerOut(value);
		}
		else if (value < mCurrentLevel) {
			emit zoomerIn(value);
		} 
		mCurrentLevel = value;
	}

	void Zoomer::onChangeSliderPosition(int level) {
		mCurrentLevel = level;
		mZoomSlider->setSliderPosition(level);
	}
}