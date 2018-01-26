#include "keypoint.h" 
#include <QVariant>

namespace timeline {
	 
	KeyPoint::KeyPoint(QWidget* parent /* = Q_NULLPTR */)
		: QWidget(parent) 
	{
		setProperty("type", "point");
		setAttribute(Qt::WA_TranslucentBackground, true);
		setCursor(Qt::PointingHandCursor); 
		setFixedSize(8, 8);   
		show();
	} 

	void KeyPoint::paintEvent(QPaintEvent *event) {

	}
}