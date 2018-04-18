#include "indicator.h"
  
namespace timeline {
	 
	Indicator::Indicator(QWidget* parent /* = Q_NULLPTR */)
		: QLabel(parent)
	{
		setAttribute(Qt::WA_TranslucentBackground, true);
		setCursor(Qt::SizeHorCursor);
		setPixmap(QPixmap(":/images/indicator"));
		setMinimumSize(19, 130);
		move(0, 0);
	} 
}