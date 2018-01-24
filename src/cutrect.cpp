#include "cutrect.h"

#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>

namespace timeline {
	 
	CutRect::CutRect(QWidget* parent /* = Q_NULLPTR */)
		: QWidget(parent)
	{
		mLeftBorder = new QToolButton(this);
		mLeftBorder->setIcon(QIcon(":/images/cutleft"));
		mLeftBorder->setFixedSize(10, 86);
		mLeftBorder->installEventFilter(this);
		mRightBorder = new QToolButton(this);
		mRightBorder->setIcon(QIcon(":/images/cutright"));
		mRightBorder->setFixedSize(10, 86);
		mRightBorder->installEventFilter(this);
		mRectBox = new QFrame(this);
		mRectBox->setObjectName("cutrect");

		QHBoxLayout* mainLayout = new QHBoxLayout(this);
		mainLayout->setSpacing(0);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->addWidget(mLeftBorder);
		mainLayout->addWidget(mRectBox, 1);
		mainLayout->addWidget(mRightBorder);
		setLayout(mainLayout);
	}
	 
	bool CutRect::eventFilter(QObject *watched, QEvent *event) {
		if (watched == mLeftBorder || watched == mRightBorder) {
			if (event->type() == QEvent::MouseMove) {
				QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
				QPoint mousePos = mouseEvent->pos();
				if (watched == mLeftBorder) {

				}
				else {

				}

				return true;
			}
			else {
				return false;
			}
		} 
		else {
			// pass the event on to the parent class
			return QWidget::eventFilter(watched, event);
		}
	}
}