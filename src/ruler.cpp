#include "ruler.h"
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QPainter>

namespace timeline {

	Ruler::Ruler(QWidget* parent /* = Q_NULLPTR */)
		: QWidget(parent),
		mOrigin(25.0),
		mRulerUnit(1.0),
		mRulerZoom(1.0),
		mRulerColor(37, 38, 39),
		mMouseTracking(true),
		mDrawText(false),
		mDuration(0.0)
	{ 
		setMouseTracking(false);
		QFont txtFont("Goudy Old Style", 12, 20);
		txtFont.setStyleHint(QFont::Helvetica, QFont::PreferAntialias);
		setFont(txtFont);

		mIndicator = new Indicator(this);

		mContextMenu = new QMenu(this);
		mClearPoints = new QAction(tr("Clear All Points"), this);
		mCutWithCurrentPos = new QAction(tr("Cut With Currrent Position"), this);
		mMakeCurrentPoint = new QAction(tr("Mark in Current Position"), this);

		connect(mIndicator, &Indicator::indicatorMove, this, &Ruler::onIndicatorMove);
		connect(mIndicator, &Indicator::indicatorPress, this, &Ruler::onIndicatorPress);
		connect(mIndicator, &Indicator::indicatorRelease, this, &Ruler::onIndicatorRelease);
	}

	void Ruler::paintEvent(QPaintEvent *event) {
		QPainter painter(this);
		painter.setRenderHints(QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);
		QPen pen(Qt::white, 0);
		painter.setPen(pen);
		// We want to work with floating point, so we are considering
		// the rect as QRectF
		QRectF rulerRect = this->rect();

		// at first fill the rect 
		painter.fillRect(rulerRect, mRulerColor);

		// drawing a scale of 25
		drawAScaleMeter(&painter, rulerRect, 25, rulerRect.height() / 2);
		// drawing a scale of 100
		mDrawText = true;
		drawAScaleMeter(&painter, rulerRect, 100, 10);
		mDrawText = false; 

		// drawing no man's land between the ruler & view
		//QPointF starPt = rulerRect.bottomLeft();
		//QPointF endPt = rulerRect.bottomRight();
		//painter.setPen(QPen(Qt::black, 2));
		//painter.drawLine(starPt, endPt);
	}

	void Ruler::setDuration(float duration) { 
		mDuration = duration;
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

	void Ruler::onIndicatorMove(const QMouseEvent& event) {

	}

	void Ruler::onIndicatorPress(const QMouseEvent& event) {

	}

	void Ruler::onIndicatorRelease(const QMouseEvent& event) {

	} 

	void Ruler::setOrigin(const qreal origin)
	{
		if (mOrigin != origin)
		{
			mOrigin = origin;
			update();
		}
	}

	void Ruler::setRulerUnit(const qreal rulerUnit)
	{
		if (mRulerUnit != rulerUnit)
		{
			mRulerUnit = rulerUnit;
			update();
		}
	}

	void Ruler::setRulerZoom(const qreal rulerZoom)
	{
		if (mRulerZoom != rulerZoom)
		{
			mRulerZoom = rulerZoom;
			update();
		}
	}
	  
	void Ruler::setMouseTrack(const bool track)
	{
		if (mMouseTracking != track)
		{
			mMouseTracking = track;
			update();
		}
	}

	void Ruler::setRulerColor(const QColor& color) {
		mRulerColor = color;
	}
	 
	void Ruler::drawAScaleMeter(QPainter* painter, QRectF rulerRect, qreal scaleMeter, qreal startPositoin)
	{
		scaleMeter = scaleMeter * mRulerUnit * mRulerZoom;

		// Ruler rectangle starting mark
		qreal rulerStartMark = rulerRect.left();
		// Ruler rectangle ending mark
		qreal rulerEndMark = rulerRect.right();

		// Condition A # If origin point is between the start & end mark,
		// we have to draw both from origin to left mark & origin to right mark.
		// Condition B # If origin point is left of the start mark, we have to draw
		// from origin to end mark.
		// Condition C # If origin point is right of the end mark, we have to draw
		// from origin to start mark.
		if (mOrigin >= rulerStartMark && mOrigin <= rulerEndMark) {
			drawFromOriginTo(painter, rulerRect, mOrigin, rulerEndMark, 0, scaleMeter, startPositoin);
			drawFromOriginTo(painter, rulerRect, mOrigin, rulerStartMark, 0, -scaleMeter, startPositoin);
		}
		else if (mOrigin < rulerStartMark) {
			int tickNo = int((rulerStartMark - mOrigin) / scaleMeter);
			drawFromOriginTo(painter, rulerRect, mOrigin + scaleMeter * tickNo,
				rulerEndMark, tickNo, scaleMeter, startPositoin);
		}
		else if (mOrigin > rulerEndMark) {
			int tickNo = int((mOrigin - rulerEndMark) / scaleMeter);
			drawFromOriginTo(painter, rulerRect, mOrigin - scaleMeter * tickNo,
				rulerStartMark, tickNo, -scaleMeter, startPositoin);
		}
	}

	void Ruler::drawFromOriginTo(QPainter* painter, QRectF rulerRect, qreal startMark, qreal endMark, int startTickNo,
		qreal step, qreal startPosition)
	{
		for (qreal current = startMark;
			(step < 0 ? current >= endMark : current <= endMark); current += step)
		{
			qreal x1 = current;
			qreal y1 = rulerRect.top() + startPosition;
			qreal x2 = current;
			qreal y2 = rulerRect.bottom();
			painter->drawLine(QLineF(x1, y1, x2, y2));
			if (mDrawText)
			{
				QPainterPath txtPath;
				txtPath.addText(x1 - 10, y1, this->font(), QString::number(qAbs(int(step) * startTickNo++)));
				painter->drawPath(txtPath);
			}
		}
	}
}


