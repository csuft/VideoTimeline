#include "qmlutilities.h"
#include "timelineitems.h"
#include <QCoreApplication>
#include <QSysInfo>
#include <QCursor>
#include <QtQml>
#include <QQmlEngine>
#include <QQmlContext>

namespace timeline {

	QmlUtilities::QmlUtilities(QObject *parent) :
		QObject(parent) {

	}

	void QmlUtilities::registerCommonTypes() {
		registerTimelineItems();
	}

	void QmlUtilities::setCommonProperties(QQmlContext* context) {
		//context->setContextProperty("settings", &ShotcutSettings::singleton());
		//context->setContextProperty("application", &QmlApplication::singleton()); 
	}

	QDir QmlUtilities::qmlDir() {
		QDir dir(qApp->applicationDirPath());
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
		dir.cdUp();
#endif
		dir.cd("qml");
		return dir;
	}

	QQmlEngine* QmlUtilities::sharedEngine() {
		static QQmlEngine * s_engine = 0;
		if (!s_engine)
			s_engine = new QQmlEngine;
		return s_engine;
	}
}

