#ifndef _CUT_RECT_H
#define _CUT_RECT_H

#include <QWidget> 
#include <QToolButton>
#include <QFrame>

namespace timeline {

	class CutRect : public QWidget {

		Q_OBJECT
	public:
		CutRect(QWidget* parent = Q_NULLPTR); 
		~CutRect() = default; 
		 
	private:
		virtual bool eventFilter(QObject *watched, QEvent *event) override;

	private:
		QToolButton* mLeftBorder;
		QToolButton* mRightBorder;
		QFrame* mRectBox;
	};
	 
}


#endif  // _CUT_RECT_H