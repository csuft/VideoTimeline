#ifndef _TIMELINE_H
#define _TIMELINE_H  

#include <QQuickWidget> 

namespace timeline {

	class Timeline : public QQuickWidget {
		Q_OBJECT
	public:
		Timeline(QWidget* parent = Q_NULLPTR);
		~Timeline() = default; 
		 
	};
}

#endif // _TIMELINE_H