#include "timeline.h"  

namespace timeline {

	Timeline::Timeline(QQmlEngine* qmlEngine, QWidget* parent /* = Q_NULLPTR */)
		: QQuickWidget(qmlEngine, parent) { 
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	}

}


