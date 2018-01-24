#include "mainwindow.h"
#include <QVBoxLayout> 
#include <QScrollBar>

namespace timeline {

	MainWindow::MainWindow(QWidget* parent /* = Q_NULLPTR */)
		: QScrollArea(parent)
	{
		setWindowIcon(QIcon(":/images/logo")); 
		resize(1000, 150);
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		horizontalScrollBar()->setRange(0, 100); 

		QVBoxLayout* mainLayout = new QVBoxLayout(this);
		mainLayout->setSpacing(0);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mZoomer = new Zoomer(this);
		addScrollBarWidget(mZoomer, Qt::AlignLeft);
		mRuler = new Ruler(this);
		mainLayout->addWidget(mRuler); 
		setLayout(mainLayout);
	}

}


