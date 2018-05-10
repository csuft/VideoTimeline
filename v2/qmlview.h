#ifndef QMLVIEW_H
#define QMLVIEW_H

#include <QObject>
#include <QPoint>

class QWidget;

namespace timeline {

	class QmlView : public QObject {
		Q_OBJECT
		Q_PROPERTY(QPoint pos READ pos);

	public:
		explicit QmlView(QWidget* qview);
		QPoint pos();

	private:
		QWidget* m_qview;
	};
}

#endif // QMLVIEW_H
