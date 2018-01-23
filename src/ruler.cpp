#include "ruler.h"

namespace timeline {

	TimelineWidget::TimelineWidget(QWidget* parent /* = Q_NULLPTR */) 
		: QWidget(parent)
	{
		setWindowIcon(QIcon(":/images/logo"));
		setFixedHeight(150);
	}

}


