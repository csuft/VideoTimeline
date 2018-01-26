#include "ruler.h"
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QPainter> 

#define HEADER_HEIGHT 40
#define BODY_HEIGHT 80
#define START_END_PADDING 60
#define MAX_INTERVAL 40
#define MIN_INTERVAL 20
#define CUT_MARKER_WIDTH 10
#define CUT_MARKER_HEIGHT 86
#define TIME_LABEL_OFFSET 10

namespace timeline {

	Ruler::Ruler(QWidget* parent /* = Q_NULLPTR */, int duration)
		: QWidget(parent),
		mOrigin(10.0),
		mBodyBgrd(37, 38, 39),
		mHeaderBgrd(32, 32, 32),
		mIntervalLength(30.0),
		mSliderLevel(1),
		mDuration(duration),
		mRectWidth(mIntervalLength * mDuration / secondsPerInterval())
	{  
		setAttribute(Qt::WA_OpaquePaintEvent);
		setupChildren();

		mContextMenu = new QMenu(this);
		mClearPoints = new QAction(tr("Clear All Points"), this);
		mCutWithCurrentPos = new QAction(tr("Cut With Currrent Position"), this);
		mMakeCurrentPoint = new QAction(tr("Mark in Current Position"), this);  

		resize(mRectWidth + START_END_PADDING, 120);
	}

	void Ruler::setupChildren() {
		mIndicator = new Indicator(this);
		mIndicator->installEventFilter(this);
		mIndicatorTime = 0;

		mBeginMarker = new QLabel(this);
		mBeginMarker->setPixmap(QPixmap(":/images/cutleft"));
		mBeginMarker->setCursor(Qt::SizeHorCursor);
		mBeginMarker->setFixedSize(CUT_MARKER_WIDTH, CUT_MARKER_HEIGHT);
		mBeginMarker->move(0, HEADER_HEIGHT);
		mBeginMarker->installEventFilter(this);
		mBeginMarkerTime = 0;

		mEndMarker = new QLabel(this);
		mEndMarker->setPixmap(QPixmap(":/images/cutright"));
		mEndMarker->setFixedSize(CUT_MARKER_WIDTH, CUT_MARKER_HEIGHT);
		mEndMarker->move(mRectWidth + CUT_MARKER_WIDTH, HEADER_HEIGHT);
		mEndMarker->setCursor(Qt::SizeHorCursor);
		mEndMarker->installEventFilter(this);
		mEndMarkerTime = (mRectWidth + CUT_MARKER_WIDTH)/lengthPerSecond();

		mRectBox = new QFrame(this);
		mRectBox->setObjectName("cutrect");
		mRectBox->setGeometry(mBeginMarker->rect().right(), mBeginMarker->y(), 
			mEndMarker->x() - mBeginMarker->rect().right(), BODY_HEIGHT);
	}

	void Ruler::resetChildren(quint32 duration) {
		mDuration = duration; 
		mRectWidth = mIntervalLength * mDuration / secondsPerInterval();
		mIndicator->move(0, 0);
		mBeginMarker->move(0, HEADER_HEIGHT);
		mEndMarker->move(mRectWidth + CUT_MARKER_WIDTH, HEADER_HEIGHT);
		mRectBox->setGeometry(mBeginMarker->rect().right(), mBeginMarker->y(),
			mEndMarker->x() - mBeginMarker->rect().right(), BODY_HEIGHT); 
	}

	void Ruler::onMoveIndicator(qreal frameTime) {

	}

	// update children when the ruler scaled up or down
	void Ruler::updateChildren() {
		mRectWidth = mIntervalLength * mDuration / secondsPerInterval();
		
		mBeginMarker->move(mBeginMarkerTime * mIntervalLength / secondsPerInterval(), mBeginMarker->y());
		mEndMarker->move(mEndMarkerTime * mIntervalLength / secondsPerInterval(), mEndMarker->y());
		mRectBox->setGeometry(mBeginMarker->x() + CUT_MARKER_WIDTH, mBeginMarker->y(),
			mEndMarker->x() - mBeginMarker->x() - CUT_MARKER_WIDTH, BODY_HEIGHT);
		mIndicator->move(mIndicatorTime * mIntervalLength / secondsPerInterval(), mIndicator->y());
		update();
	}

	bool Ruler::eventFilter(QObject *watched, QEvent *event) {
		if (watched == mIndicator || watched == mBeginMarker 
			|| watched == mEndMarker)
		{
			static QPoint lastPnt;
			static bool isHover = false;
			if (event->type() == QEvent::MouseButtonPress) { 
				QLabel* control = dynamic_cast<QLabel*>(watched);
				QMouseEvent* e = static_cast<QMouseEvent*>(event);
				if (control->rect().contains(e->pos()) &&  
					(e->button() == Qt::LeftButton)) {
					lastPnt = e->pos();
					isHover = true;
				}
			}
			else if (event->type() == QEvent::MouseMove && isHover) {
				QMouseEvent* e = dynamic_cast<QMouseEvent*>(event);
				int dx = e->pos().x() - lastPnt.x();
				int dy = e->pos().y() - lastPnt.y();

				if (watched == mIndicator) {
					if (mIndicator->x() + dx <= mRectWidth - CUT_MARKER_WIDTH &&
						mIndicator->x() + dx >= 0) {
						mIndicatorTime = (mIndicator->x() + dx)/lengthPerSecond();
						mIndicator->move(mIndicator->x() + dx, mIndicator->y());
					}
				}
				if (watched == mBeginMarker) {
					if (mBeginMarker->x() + dx + CUT_MARKER_WIDTH <= mRectWidth &&
						mBeginMarker->x() + dx >= 0 &&
						mBeginMarker->x() + dx + CUT_MARKER_WIDTH <= mEndMarker->x()) {
						mBeginMarkerTime = (mBeginMarker->x() + dx)/lengthPerSecond();
						mBeginMarker->move(mBeginMarker->x() + dx, mBeginMarker->y());
						updateRectBox();
					}
				}
				if (watched == mEndMarker) {
					if (mEndMarker->x() + dx <= mRectWidth + CUT_MARKER_WIDTH &&
						mEndMarker->x() + dx >= 0 &&
						mEndMarker->x() + dx - CUT_MARKER_WIDTH >= mBeginMarker->x()) {
						mEndMarkerTime = (mEndMarker->x() + dx)/lengthPerSecond();
						mEndMarker->move(mEndMarker->x() + dx, mEndMarker->y());
						updateRectBox();
					}
				}
			}
			else if (event->type() == QEvent::MouseButtonRelease && isHover) {
				isHover = false;
			}
		}

		return false;
	} 

	void Ruler::updateRectBox() { 
		mRectBox->setGeometry(mBeginMarker->x() + CUT_MARKER_WIDTH, mBeginMarker->y(), 
			mEndMarker->x() - mBeginMarker->x() - CUT_MARKER_WIDTH, BODY_HEIGHT);
	}

	void Ruler::contextMenuEvent(QContextMenuEvent *event) {
		mContextMenu->addAction(mClearPoints);
		mContextMenu->addAction(mCutWithCurrentPos);
		mContextMenu->addAction(mMakeCurrentPoint);
		mContextMenu->exec(QCursor::pos());
		event->accept();
	}  

	void Ruler::wheelEvent(QWheelEvent *event) {
		QPoint numDegrees = event->angleDelta() / 8; 
		if (!numDegrees.isNull()) {
			if (numDegrees.y() > 0) {
				onZoomerIn(mSliderLevel);
			}
			if (numDegrees.y() < 0)
			{
				onZoomerOut(mSliderLevel);
			}
			emit changeSliderPosition(mSliderLevel);
		}
		event->accept();
	}

	void Ruler::mousePressEvent(QMouseEvent* event) {

	}

	void Ruler::mouseReleaseEvent(QMouseEvent* event) {

	} 
	 
	void Ruler::paintEvent(QPaintEvent *event) { 
		QPainter painter(this);
		QFont font = painter.font();
		font.setPointSize(8);
		painter.setFont(font);
		painter.setRenderHints(QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);

		QRectF rulerRect = this->rect(); 
		// paint header background color
		painter.fillRect(QRect(rulerRect.left(), rulerRect.top(), 
			rulerRect.width(), HEADER_HEIGHT), mHeaderBgrd);
		// paint body background color
		painter.fillRect(QRect(rulerRect.left(), rulerRect.top() + HEADER_HEIGHT, 
			rulerRect.width(), rulerRect.height() - HEADER_HEIGHT), mBodyBgrd);

		if (mDuration) {
			// draw tickers and time labels
			drawScaleRuler(&painter, rulerRect);
		} 
	}

	qreal Ruler::lengthPerSecond() {
		switch (mSliderLevel)
		{
		case 1:
			return mIntervalLength / 16.0;
		case 2:
			return mIntervalLength / 8.0;
		case 3:
			return mIntervalLength / 4.0;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		default:
			return mIntervalLength / 2.0; 
		}
	}
	int Ruler::secondsPerInterval() { 
		switch (mSliderLevel)
		{
		case 1:
			return 16; 
		case 2:
			return 8; 
		case 3:
			return 4; 
		case 4: 
		case 5:
		case 6:
		case 7:
		case 8: 
		default:
			return 2; 
		}
	}
	
	QString Ruler::getTickerString(qreal currentPos) {
		qreal pos = currentPos - mOrigin;
		int intervalNums = pos / mIntervalLength;
		if (intervalNums == 0) {
			return "00:00";
		}
		QTime currentTime(intervalNums * secondsPerInterval() / 3600, 
			intervalNums * secondsPerInterval() / 60,
			intervalNums * secondsPerInterval() % 60);
		
		if (intervalNums % 2 == 0) {
			return currentTime.toString("mm:ss");
		}

		return "";
	}

	void Ruler::onZoomerIn(int level) {
		mSliderLevel = level;
		if (mIntervalLength > MIN_INTERVAL) {
			mIntervalLength -= 2;
			updateChildren();
		} 
	}

	void Ruler::onZoomerOut(int level) {
		mSliderLevel = level;
		if (mIntervalLength < MAX_INTERVAL) {
			mIntervalLength += 2;
			updateChildren();
		} 
	} 

	void Ruler::drawScaleRuler(QPainter* painter, QRectF rulerRect) { 
		qreal rulerStartMark = rulerRect.left();
		qreal rulerEndMark = rulerRect.right();

		for (qreal current = mOrigin; current <= rulerEndMark; current += 2* mIntervalLength)
		{
			qreal x1 = current;
			qreal y1 = rulerRect.top() + HEADER_HEIGHT - 5;
			qreal x2 = current;
			qreal y2 = rulerRect.bottom();

			// draw 2 tickers within one circle.
			QPen tickerPen(QColor(61, 61, 61), 1);
			painter->setPen(tickerPen);
			painter->drawLine(QLineF(x1, y1, x2, y2));
			if (x1 + mIntervalLength <= rulerEndMark) {
				painter->drawLine(QLineF(x1 + mIntervalLength, y1, x2 + mIntervalLength, y2));
			}
			
			// draw 2 time text within one circle.
			QPen textPen(QColor(121, 121, 121), 1);
			painter->setPen(textPen);
			painter->drawText(x1 - TIME_LABEL_OFFSET, y1 - HEADER_HEIGHT / 4, getTickerString(x1));
			if (x1 + mIntervalLength - TIME_LABEL_OFFSET <= rulerEndMark) {
				painter->drawText(x1 + mIntervalLength - TIME_LABEL_OFFSET, y1 - HEADER_HEIGHT / 4,
					getTickerString(x1 + mIntervalLength));
			}
			
		}
	} 
}


