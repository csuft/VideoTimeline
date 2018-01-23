#ifndef _RULER_H
#define _RULER_H

#include <QWidget>
#include "indicator.h" 

namespace timeline {

	class Ruler : public QWidget {
		Q_OBJECT
	public:
		Ruler(QWidget* parent = Q_NULLPTR);
		~Ruler() = default;

	protected:
		virtual void paintEvent(QPaintEvent *event) override;
		virtual void contextMenuEvent(QContextMenuEvent *event) override; 

	private:
		void onIndicatorMove(const QMouseEvent& event);
		void onIndicatorPress(const QMouseEvent& event);
		void onIndicatorRelease(const QMouseEvent& event);

	private: 
		Indicator* mIndicator;
	};
}

#endif // _RULER_H