#ifndef _INDICATOR_H
#define _INDICATOR_H

#include <QWidget>

namespace timeline {

	class Indicator : public QWidget {

		Q_OBJECT
	public:
		Indicator(QWidget* parent = Q_NULLPTR);
		~Indicator() = default;

	protected:
		virtual void paintEvent(QPaintEvent *event) override; 
		virtual void mouseMoveEvent(QMouseEvent *event) override;
		virtual void mousePressEvent(QMouseEvent *event) override;
		virtual void mouseReleaseEvent(QMouseEvent *event) override;

	signals:
		void indicatorMove(const QMouseEvent& event);
		void indicatorPress(const QMouseEvent& event);
		void indicatorRelease(const QMouseEvent& event);
	};

}


#endif  // _INDICATOR_H