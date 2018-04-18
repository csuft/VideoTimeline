#ifndef _KEY_LINE_H
#define _KEY_LINE_H

#include <QFrame>  

namespace timeline {

	class KeyLine : public QFrame {

		Q_OBJECT
	public:
		KeyLine(QWidget* parent = Q_NULLPTR); 
		~KeyLine() = default; 

	protected:
		virtual void paintEvent(QPaintEvent *event) override;
	}; 
}


#endif  // _KEY_LINE_H