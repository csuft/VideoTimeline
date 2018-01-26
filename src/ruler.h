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
		explicit Ruler(QWidget* parent = Q_NULLPTR, qreal frameRate = 30.0);
		~Ruler() = default;

		inline void setDuration(int duration) {
			mTotalSeconds = duration;
		}
		void setOrigin(const qreal origin);   
		inline void setHeaderColor(const QColor& color) {
			mHeaderBgrd = color;
		}
		inline void setBodyColor(const QColor& color) {
			mBodyBgrd = color;
		}

	signals:
		void changeSliderPosition(int level);

	public slots:
		void onZoomerIn(int level);
		void onZoomerOut(int level);
		void onTimeOut();

	protected:
		virtual void paintEvent(QPaintEvent *event) override;
		virtual void contextMenuEvent(QContextMenuEvent *event) override;  
		virtual void wheelEvent(QWheelEvent *event) override;
		virtual bool eventFilter(QObject *watched, QEvent *event) override;
		virtual void mousePressEvent(QMouseEvent* event) override;
		virtual void mouseReleaseEvent(QMouseEvent* event) override;

	private: 
		void initializeChildren();
		void drawScaleRuler(QPainter* painter, QRectF rulerRect); 
		QString getTickerString(qreal tickerNo);
		int secondsPerInterval();
		void updateRectBox();

	private: 
		// sub controls
		Indicator* mIndicator;
		QLabel* mLeftBorder;
		QLabel* mRightBorder;
		QFrame* mRectBox;
		int mSliderLevel;

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
		quint32 mTotalSeconds;

		// updater
		QTimer* mUpdater;
		qreal mFrameRate;
	};
}

#endif // _RULER_H