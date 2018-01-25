#ifndef _INDICATOR_H
#define _INDICATOR_H

#include <QLabel>  

namespace timeline {

	class Indicator : public QLabel {

		Q_OBJECT
	public:
		Indicator(QWidget* parent = Q_NULLPTR);
		~Indicator() = default;
	};
	 
}


#endif  // _INDICATOR_H