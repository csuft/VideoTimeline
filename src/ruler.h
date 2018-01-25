#ifndef _RULER_H
#define _RULER_H

#include <QWidget>
#include <QTime> 
#include <QLabel> 

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QMenu) 

namespace timeline {

	class Ruler : public QWidget {
		Q_OBJECT 
		Q_PROPERTY(qreal origin READ origin WRITE setOrigin)
		Q_PROPERTY(qreal rulerUnit READ rulerUnit WRITE setRulerUnit)
		Q_PROPERTY(qreal rulerZoom READ rulerZoom WRITE setRulerZoom)
	public:
		explicit Ruler(QWidget* parent = Q_NULLPTR);
		~Ruler() = default;

		inline void setDuration(QTime duration) {
			mDuration = duration;
		}
		inline qreal origin() const {
			return mOrigin;
		}
		inline qreal rulerUnit() const {
			return mRulerUnit;
		}
		inline qreal rulerZoom() const {
			return mRulerZoom;
		}
		void setOrigin(const qreal origin);
		void setRulerUnit(const qreal rulerUnit);
		void setRulerZoom(const qreal rulerZoom); 
		void setMouseTrack(const bool track); 
		void setHeaderColor(const QColor& color) {
			mHeaderBgrd = color;
		}
		void setBodyColor(const QColor& color) {
			mBodyBgrd = color;
		}

	protected:
		virtual void paintEvent(QPaintEvent *event) override;
		virtual void contextMenuEvent(QContextMenuEvent *event) override; 
		virtual void mouseMoveEvent(QMouseEvent *event) override;
		virtual bool eventFilter(QObject *watched, QEvent *event) override;

	private: 
		void drawScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin);
		void drawTickers(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, 
			int startTickNo, qreal step, qreal startPosition);
		void getTickerString(int tickerNo);

	private: 
		// sub controls
		QLabel* mIndicator;
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
		qreal mRulerUnit;
		qreal mRulerZoom;
		QPoint mCursorPos;
		bool mMouseTracking;
		bool mDrawText;
		QColor mBodyBgrd;
		QColor mHeaderBgrd;
		QTime mDuration;
	};
}

#endif // _RULER_H