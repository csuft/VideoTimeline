#ifndef _RULER_H
#define _RULER_H

#include <QWidget>
#include "indicator.h"
#include "zoomer.h"

namespace timeline {

	class TimelineWidget : public QWidget {
		Q_OBJECT
	public:
		TimelineWidget(QWidget* parent = Q_NULLPTR);
		~TimelineWidget() = default;

	
	};
}

#endif