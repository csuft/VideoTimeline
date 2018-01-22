#ifndef _INDICATOR_H
#define _INDICATOR_H

#include <QWidget>

namespace timeline {

	class Indicator : public QWidget {

		Q_OBJECT
	public:
		Indicator(QWidget* parent = Q_NULLPTR);
		~Indicator() = default;
	};

}


#endif