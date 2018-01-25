#ifndef _RULER_H
#define _RULER_H

#include <QWidget>
#include <QTime> 
#include "indicator.h" 

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QMenu) 

namespace timeline {

	class Ruler : public QWidget {
		Q_OBJECT 
		Q_PROPERTY(qreal origin READ origin WRITE setOrigin) 
	public:
		explicit Ruler(QWidget* parent = Q_NULLPTR);
		~Ruler() = default;

		inline void setDuration(QTime duration) {
			mDuration = duration;
		}
		inline qreal origin() const {
			return mOrigin;
		}  
		void setOrigin(const qreal origin);   
		inline void setHeaderColor(const QColor& color) {
			mHeaderBgrd = color;
		}
		inline void setBodyColor(const QColor& color) {
			mBodyBgrd = color;
		}

	public slots:
		void onZoomerIn();
		void onZoomerOut();

	protected:
		virtual void paintEvent(QPaintEvent *event) override;
		virtual void contextMenuEvent(QContextMenuEvent *event) override; 
		virtual void mouseMoveEvent(QMouseEvent *event) override;
		virtual void wheelEvent(QWheelEvent *event) override;
		virtual bool eventFilter(QObject *watched, QEvent *event) override;

	private: 
		void drawScaleRuler(QPainter* painter, QRectF rulerRect);
		void drawTickers(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark);
		QString getTickerString(qreal tickerNo);

	private: 
		// sub controls
		Indicator* mIndicator;
		QLabel* mLeftBorder;
		QLabel* mRightBorder;
		QFrame* mRectBox;

		// context menu
		QMenu* mContextMenu;
		QAction* mClearPoints;
		QAction* mMakeCurrentPoint;
		QAction* mCutWithCurrentPos; 

		// ruler members
		qreal mOrigin;  
		qreal mInterval;
		QPoint mCursorPos;  
		QColor mBodyBgrd;
		QColor mHeaderBgrd;
		QTime mDuration;
	};
}

#endif // _RULER_H