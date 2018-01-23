#include "mainwindow.h"
#include <QVBoxLayout>

namespace timeline {

	MainWindow::MainWindow(QWidget* parent /* = Q_NULLPTR */)
		: QWidget(parent)
	{
		setWindowIcon(QIcon(":/images/logo"));
		setFixedHeight(150);

		QVBoxLayout* mainLayout = new QVBoxLayout(this);
		mainLayout->setSpacing(0);
		mainLayout->setContentsMargins(0, 0, 0, 0);

		mZoomer = new Zoomer(this);
		mRuler = new Ruler(this);
		mainLayout->addWidget(mRuler);
		mainLayout->addWidget(mZoomer);
		setLayout(mainLayout);
	}

}


