#ifndef _KEY_POINT_H
#define _KEY_POINT_H

#include <QWidget>  

namespace timeline {

	class KeyPoint : public QWidget { 
		Q_OBJECT
	public:
		KeyPoint(QWidget* parent = Q_NULLPTR); 
		~KeyPoint() = default; 

	protected:
		virtual void paintEvent(QPaintEvent *event) override;
	};
	 
} 

#endif  // _KEY_POINT_H