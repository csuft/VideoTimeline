#ifndef QMLUTILITIES_H
#define QMLUTILITIES_H

#include <QObject>
#include <QDir>
#include <QPoint>
#include <QUrl>

class QQmlContext;
class QQmlEngine;

namespace timeline {

	class QmlUtilities : public QObject {
		Q_OBJECT

	public:
		explicit QmlUtilities(QObject *parent = 0);

		static void registerCommonTypes();
		static void setCommonProperties(QQmlContext* context);
		static QDir qmlDir();
		static QQmlEngine * sharedEngine();
	};

}



#endif // QMLUTILITIES_H
