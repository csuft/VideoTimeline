#include "ruler.h"
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QPainter>
#include <QDebug> 

#define HEADER_HEIGHT 40
#define BODY_HEIGHT 80
#define START_END_PADDING 60
#define MAX_INTERVAL 40
#define MIN_INTERVAL 20
#define CUT_MARKER_WIDTH 10
#define CUT_MARKER_HEIGHT 86
#define TIME_LABEL_OFFSET 10

namespace timeline {

	Ruler::Ruler(QWidget* parent /* = Q_NULLPTR */, qreal frameRate)
		: QWidget(parent),
		mOrigin(10.0),
		mBodyBgrd(37, 38, 39),
		mHeaderBgrd(32, 32, 32),
		mIntervalLength(30.0),
		mSliderLevel(1),
		mTotalSeconds(126),
		mRectWidth(mIntervalLength*mTotalSeconds/secondsPerInterval())
	{  
		setAttribute(Qt::WA_OpaquePaintEvent);
		initializeChildren();

		mContextMenu = new QMenu(this);
		mClearPoints = new QAction(tr("Clear All Points"), this);
		mCutWithCurrentPos = new QAction(tr("Cut With Currrent Position"), this);
		mMakeCurrentPoint = new QAction(tr("Mark in Current Position"), this); 

		mUpdater = new QTimer(this);
		if (mFrameRate == 0.0) {
			mFrameRate = 30.0;
		}
		mUpdater->setInterval(1000/mFrameRate);
		mUpdater->setSingleShot(false);
		connect(mUpdater, &QTimer::timeout, this, &Ruler::onTimeOut);

		resize(mRectWidth + START_END_PADDING, 120);
	}

	void Ruler::initializeChildren() {
		mIndicator = new Indicator(this);
		mIndicator->installEventFilter(this);

		mLeftBorder = new QLabel(this);
		mLeftBorder->setPixmap(QPixmap(":/images/cutleft"));
		mLeftBorder->setCursor(Qt::SizeHorCursor);
		mLeftBorder->setFixedSize(CUT_MARKER_WIDTH, CUT_MARKER_HEIGHT);
		mLeftBorder->move(0, HEADER_HEIGHT);
		mLeftBorder->installEventFilter(this);

		mRightBorder = new QLabel(this);
		mRightBorder->setPixmap(QPixmap(":/images/cutright"));
		mRightBorder->setFixedSize(CUT_MARKER_WIDTH, CUT_MARKER_HEIGHT);
		mRightBorder->move(mRectWidth + CUT_MARKER_WIDTH, HEADER_HEIGHT);
		mRightBorder->setCursor(Qt::SizeHorCursor);
		mRightBorder->installEventFilter(this);

		mRectBox = new QFrame(this);
		mRectBox->setObjectName("cutrect");
		mRectBox->setGeometry(mLeftBorder->rect().right(), mLeftBorder->y(), mRightBorder->x() - mLeftBorder->rect().right(), BODY_HEIGHT);
	}

	bool Ruler::eventFilter(QObject *watched, QEvent *event) {
		if (watched == mIndicator || watched == mLeftBorder 
			|| watched == mRightBorder)
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
						mIndicator->move(mIndicator->x() + dx, mIndicator->y());
					}
				}
				if (watched == mLeftBorder) {
					if (mLeftBorder->x() + dx + CUT_MARKER_WIDTH <= mRectWidth &&
						mLeftBorder->x() + dx >= 0 &&
						mLeftBorder->x() + dx + CUT_MARKER_WIDTH <= mRightBorder->x()) {
						mLeftBorder->move(mLeftBorder->x() + dx, mLeftBorder->y());
						updateRectBox();
					}
				}
				if (watched == mRightBorder) {
					if (mRightBorder->x() + dx <= mRectWidth + CUT_MARKER_WIDTH &&
						mRightBorder->x() + dx >= 0 &&
						mRightBorder->x() + dx - CUT_MARKER_WIDTH >= mLeftBorder->x()) {
						mRightBorder->move(mRightBorder->x() + dx, mRightBorder->y());
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
		mRectBox->setGeometry(mLeftBorder->x() + CUT_MARKER_WIDTH, mLeftBorder->y(), 
			mRightBorder->x() - mLeftBorder->x() - CUT_MARKER_WIDTH, BODY_HEIGHT);
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

	void Ruler::setOrigin(const qreal origin)
	{
		if (mOrigin != origin) {
			mOrigin = origin;
			update();
		}
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

		if (mTotalSeconds) {
			// draw tickers and time labels
			drawScaleRuler(&painter, rulerRect);
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
			// fix me: resize widget
		} 
	}

	void Ruler::onZoomerOut(int level) {
		mSliderLevel = level;
		if (mIntervalLength < MAX_INTERVAL) {
			mIntervalLength += 2;
			// fix me: resize widget
		} 
	}

	void Ruler::onTimeOut() {

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


