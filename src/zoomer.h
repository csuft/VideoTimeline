#ifndef _ZOOMER_H
#define _ZOOMER_H

#include <QWidget>
#include <QScrollBar>
#include <QToolButton>
#include <QSlider>

namespace timeline {

	class Zoomer : public QWidget {

		Q_OBJECT
	public:
		Zoomer(QWidget* parent = Q_NULLPTR);
		~Zoomer() = default;

	private:
		QScrollBar* mScrollBar;
		QToolButton* mZoomIn;
		QToolButton* mZoomOut;
		QSlider* mZoomSlider;
	};
	 
}


#endif