#ifndef _ZOOMER_H
#define _ZOOMER_H

#include <QWidget> 
#include <QToolButton>
#include <QSlider>

namespace timeline {

	class Zoomer : public QWidget {

		Q_OBJECT
	public:
		Zoomer(QWidget* parent = Q_NULLPTR);
		void initializeControls();
		~Zoomer() = default;

	signals:
		void zoomerChanged(int value);

	private slots:
		void onZoomInClicked(bool checked);
		void onZoomOutClicked(bool checked); 

	private: 
		QToolButton* mZoomIn;
		QToolButton* mZoomOut;
		QSlider* mZoomSlider;
	};
	 
}


#endif