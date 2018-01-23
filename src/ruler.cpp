#include "ruler.h"

namespace timeline {

	Ruler::Ruler(QWidget* parent /* = Q_NULLPTR */)
		: QWidget(parent)
	{ 
		mIndicator = new Indicator(this);

		connect(mIndicator, &Indicator::indicatorMove, this, &Ruler::onIndicatorMove);
		connect(mIndicator, &Indicator::indicatorPress, this, &Ruler::onIndicatorPress);
		connect(mIndicator, &Indicator::indicatorRelease, this, &Ruler::onIndicatorRelease);
	}

	void Ruler::paintEvent(QPaintEvent *event) {

	}

	void Ruler::contextMenuEvent(QContextMenuEvent *event) {

	} 

	void Ruler::onIndicatorMove(const QMouseEvent& event) {

	}

	void Ruler::onIndicatorPress(const QMouseEvent& event) {

	}

	void Ruler::onIndicatorRelease(const QMouseEvent& event) {

	}
}


