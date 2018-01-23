#ifndef _RULER_H
#define _RULER_H

#include <QWidget>
#include "indicator.h" 

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

		void setRulerWidth(int width); 
		qreal origin() const {
			return mOrigin;
		}
		qreal rulerUnit() const {
			return mRulerUnit;
		}
		qreal rulerZoom() const {
			return mRulerZoom;
		}
		void setOrigin(const qreal origin);
		void setRulerUnit(const qreal rulerUnit);
		void setRulerZoom(const qreal rulerZoom); 
		void setMouseTrack(const bool track); 
		void setRulerColor(const QColor& color); 

	protected:
		virtual void paintEvent(QPaintEvent *event) override;
		virtual void contextMenuEvent(QContextMenuEvent *event) override; 
		virtual void mouseMoveEvent(QMouseEvent *event) override;

	private:
		void onIndicatorMove(const QMouseEvent& event);
		void onIndicatorPress(const QMouseEvent& event);
		void onIndicatorRelease(const QMouseEvent& event); 
		void drawAScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin);
		void drawFromOriginTo(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, 
			int startTickNo, qreal step, qreal startPosition);
	private: 
		// context menu
		Indicator* mIndicator;
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
		QColor mRulerColor;
	};
}

#endif // _RULER_H