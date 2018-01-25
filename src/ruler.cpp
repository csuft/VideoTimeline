#include "ruler.h"
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QPainter>
#include <QApplication>
#include <QStyleOptionFocusRect>

#define HEADER_HEIGHT 40

namespace timeline {

	Ruler::Ruler(QWidget* parent /* = Q_NULLPTR */)
		: QWidget(parent),
		mOrigin(19.0),
		mRulerUnit(1.0),
		mRulerZoom(1.0),
		mBodyBgrd(37, 38, 39),
		mHeaderBgrd(32, 32, 32),
		mMouseTracking(true),
		mDrawText(false)
	{ 
		setMouseTracking(false); 
		mIndicator = new QLabel(this);
		mIndicator->setCursor(Qt::SizeHorCursor);
		mIndicator->move(0, 0);
		mIndicator->setPixmap(QPixmap(":/images/indicator"));
		mIndicator->setMinimumSize(19, 130);
		mIndicator->installEventFilter(this);

		mLeftBorder = new QLabel(this);
		mLeftBorder->setPixmap(QPixmap(":/images/cutleft"));
		mLeftBorder->setCursor(Qt::SizeHorCursor);
		mLeftBorder->setFixedSize(10, 86);
		mLeftBorder->move(0, HEADER_HEIGHT);
		mLeftBorder->installEventFilter(this);

		mRightBorder = new QLabel(this);
		mRightBorder->setPixmap(QPixmap(":/images/cutright"));
		mRightBorder->setFixedSize(10, 86);
		mRightBorder->move(800, HEADER_HEIGHT);
		mRightBorder->setCursor(Qt::SizeHorCursor);
		mRightBorder->installEventFilter(this);

		mRectBox = new QFrame(this);
		mRectBox->setObjectName("cutrect");
		mRectBox->setGeometry(87, 40, 700, 80);

		mContextMenu = new QMenu(this);
		mClearPoints = new QAction(tr("Clear All Points"), this);
		mCutWithCurrentPos = new QAction(tr("Cut With Currrent Position"), this);
		mMakeCurrentPoint = new QAction(tr("Mark in Current Position"), this); 
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
					if (mIndicator->x() + dx < this->width() &&
						mIndicator->x() + dx > 0) {
						mIndicator->move(mIndicator->x() + dx, mIndicator->y());
					}
				}
				if (watched == mLeftBorder) {
					if (mLeftBorder->x() + dx <= this->width() &&
						mLeftBorder->x() + dx >= 0 &&
						mLeftBorder->x() + dx <= mRightBorder->x()) {
						mLeftBorder->move(mLeftBorder->x() + dx, mLeftBorder->y());
					}
				}
				if (watched == mRightBorder) {
					if (mRightBorder->x() + dx <= this->width() &&
						mRightBorder->x() + dx >= 0 &&
						mRightBorder->x() + dx >= mLeftBorder->x()) {
						mRightBorder->move(mRightBorder->x() + dx, mRightBorder->y());
					}
				}
			}
			else if (event->type() == QEvent::MouseButtonRelease && isHover)
			{
				isHover = false;
			}
		}

		return false;
	} 

	void Ruler::contextMenuEvent(QContextMenuEvent *event) {
		mContextMenu->addAction(mClearPoints);
		mContextMenu->addAction(mCutWithCurrentPos);
		mContextMenu->addAction(mMakeCurrentPoint);
		mContextMenu->exec(QCursor::pos());
		event->accept();
	} 

	void Ruler::mouseMoveEvent(QMouseEvent *event) {
		mCursorPos = event->pos();
		update();
		QWidget::mouseMoveEvent(event);
	} 

	void Ruler::setOrigin(const qreal origin)
	{
		if (mOrigin != origin) {
			mOrigin = origin;
			update();
		}
	}

	void Ruler::setRulerUnit(const qreal rulerUnit) {
		if (mRulerUnit != rulerUnit) {
			mRulerUnit = rulerUnit;
			update();
		}
	}

	void Ruler::setRulerZoom(const qreal rulerZoom)
	{
		if (mRulerZoom != rulerZoom) {
			mRulerZoom = rulerZoom;
			update();
		}
	}
	  
	void Ruler::setMouseTrack(const bool track)
	{
		if (mMouseTracking != track) {
			mMouseTracking = track;
			update();
		}
	}  
	 
	void Ruler::paintEvent(QPaintEvent *event) {
		QPainter painter(this);
		QFont font = painter.font();
		font.setPointSize(10);
		painter.setFont(font);
		painter.setRenderHints(QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);
		QPen pen(QColor(181, 181, 181), 1);
		painter.setPen(pen); 

		QRectF rulerRect = this->rect(); 
		// paint header background color
		painter.fillRect(QRect(rulerRect.left(), rulerRect.top(), 
			rulerRect.width(), HEADER_HEIGHT), mHeaderBgrd);
		// paint body background color
		painter.fillRect(QRect(rulerRect.left(), rulerRect.top() + HEADER_HEIGHT, 
			rulerRect.width(), rulerRect.height() - HEADER_HEIGHT), mBodyBgrd);
		drawScaleMeter(&painter, rulerRect, 25, HEADER_HEIGHT);
		// drawing a scale of 100
		mDrawText = true;
		drawScaleMeter(&painter, rulerRect, 100, HEADER_HEIGHT);
		mDrawText = false;

		QWidget::paintEvent(event);
	}
	
	void Ruler::getTickerString(int tickerNo) {


	}

	void Ruler::drawScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin) {
		scaleMeter = scaleMeter * mRulerUnit * mRulerZoom;
		qreal rulerStartMark = rulerRect.left();
		qreal rulerEndMark = rulerRect.right();

		// Condition A # If origin point is between the start & end mark,
		// we have to draw both from origin to left mark & origin to right mark.
		// Condition B # If origin point is left of the start mark, we have to draw
		// from origin to end mark.
		// Condition C # If origin point is right of the end mark, we have to draw
		// from origin to start mark.
		if (mOrigin >= rulerStartMark && mOrigin <= rulerEndMark) {
			drawTickers(painter, rulerRect, mOrigin, rulerEndMark, 0, scaleMeter, startPositoin);
			drawTickers(painter, rulerRect, mOrigin, rulerStartMark, 0, -scaleMeter, startPositoin);
		}
		else if (mOrigin < rulerStartMark) {
			int tickNo = int((rulerStartMark - mOrigin) / scaleMeter);
			drawTickers(painter, rulerRect, mOrigin + scaleMeter * tickNo,
				rulerEndMark, tickNo, scaleMeter, startPositoin);
		}
		else if (mOrigin > rulerEndMark) {
			int tickNo = int((mOrigin - rulerEndMark) / scaleMeter);
			drawTickers(painter, rulerRect, mOrigin - scaleMeter * tickNo,
				rulerStartMark, tickNo, -scaleMeter, startPositoin);
		}
	}

	void Ruler::drawTickers(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo,
		qreal step, qreal startPosition)
	{
		for (qreal current = startMark;
			(step < 0 ? current >= endMark : current <= endMark); current += step)
		{
			qreal x1 = current;
			qreal y1 = rulerRect.top() + startPosition - 5;
			qreal x2 = current;
			qreal y2 = rulerRect.bottom() - HEADER_HEIGHT;
			painter->drawLine(QLineF(x1, y1, x2, y2));
			if (mDrawText) { 
				painter->drawText(x1 - 10, y1 - HEADER_HEIGHT/4, QString::number(qAbs(int(step) * startTickNo++)));
			}
		}
	}
}


