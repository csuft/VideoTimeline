#ifndef SCRUBBAR_H
#define SCRUBBAR_H

#include <QWidget>

namespace timeline {

	class ScrubBar : public QWidget {
		Q_OBJECT
			enum controls {
			CONTROL_NONE,
			CONTROL_HEAD,
			CONTROL_IN,
			CONTROL_OUT
		};

	public:
		explicit ScrubBar(QWidget *parent = 0);

		virtual void mousePressEvent(QMouseEvent * event);
		virtual void mouseReleaseEvent(QMouseEvent * event);
		virtual void mouseMoveEvent(QMouseEvent * event);
		void setScale(int maximum);
		void setFramerate(double fps);
		int position() const;
		void setInPoint(int in);
		void setOutPoint(int out);
		void setMarkers(const QList<int>&);
		QList<int> markers() const {
			return m_markers;
		}

	signals:
		void seeked(int);
		void inChanged(int);
		void outChanged(int);

	public slots:
		bool onSeek(int value);

	protected:
		virtual void paintEvent(QPaintEvent *e);
		virtual void resizeEvent(QResizeEvent *);
		virtual bool event(QEvent *event);

	private:
		int m_cursorPosition;
		int m_head;
		double m_scale;
		double m_fps;
		int m_interval;
		int m_max;
		int m_in;
		int m_out;
		enum controls m_activeControl;
		QPixmap m_pixmap;
		int m_timecodeWidth;
		int m_secondsPerTick;
		QList<int> m_markers;

		void updatePixmap();
	};

}


#endif // SCRUBBAR_H
