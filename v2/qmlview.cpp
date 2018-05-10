#include "qmlview.h"
#include <QWidget> 

namespace timeline {

	QmlView::QmlView(QWidget* qview)
		: QObject(qview)
		, m_qview(qview) {
	}

	QPoint QmlView::pos() {
		return m_qview->mapToGlobal(QPoint(0, 0));
	}

}

