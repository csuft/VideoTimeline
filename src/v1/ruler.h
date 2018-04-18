#ifndef _RULER_H
#define _RULER_H

#include <QWidget>
#include <QTimer>
#include <QTime> 
#include "indicator.h" 

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QMenu) 

namespace timeline {

	class Ruler : public QWidget {
		Q_OBJECT  
	public:
		explicit Ruler(QWidget* parent = Q_NULLPTR, int duration = 126);
		~Ruler() = default;  

		inline void setHeaderColor(const QColor& color) {
			mHeaderBgrd = color;
		}

		inline void setDuration(quint32 duration) {
			resetChildren(duration);
		}

		inline void setBodyColor(const QColor& color) {
			mBodyBgrd = color;
		}

		inline int beginTime() const {
			return mBeginMarkerTime;
		}

		inline int endTime() const {
			return mEndMarkerTime;
		}

	signals:
		void changeSliderPosition(int level);

	public slots:
		void onZoomerIn(int level);
		void onZoomerOut(int level); 
		void onMoveIndicator(qreal frameTime);

	protected:
		virtual void paintEvent(QPaintEvent *event) override;
		virtual void contextMenuEvent(QContextMenuEvent *event) override;  
		virtual void wheelEvent(QWheelEvent *event) override;
		virtual bool eventFilter(QObject *watched, QEvent *event) override;
		virtual void mousePressEvent(QMouseEvent* event) override;
		virtual void mouseReleaseEvent(QMouseEvent* event) override;

	private: 
		void setupChildren();
		void resetChildren(quint32 duration);
		void updateChildren();
		void drawScaleRuler(QPainter* painter, QRectF rulerRect); 
		QString getTickerString(qreal tickerNo);
		int secondsPerInterval();
		qreal lengthPerSecond();
		void updateRectBox();

	private: 
		// sub controls
		Indicator* mIndicator;
		QLabel* mBeginMarker;
		QLabel* mEndMarker;
		QFrame* mRectBox;
		int mSliderLevel; 
		qreal mBeginMarkerTime;  // time unit/s
		qreal mEndMarkerTime; // time unit/s
		qreal mIndicatorTime;   // time unit/s

		// context menu
		QMenu* mContextMenu;
		QAction* mClearPoints;
		QAction* mMakeCurrentPoint;
		QAction* mCutWithCurrentPos; 

		// ruler members
		qreal mOrigin;  
		qreal mIntervalLength;
		QPoint mCursorPos;  
		QColor mBodyBgrd;
		QColor mHeaderBgrd; 
		quint32 mDuration;  // time unit/s
		qreal mRectWidth; 
	};
}

#endif // _RULER_H