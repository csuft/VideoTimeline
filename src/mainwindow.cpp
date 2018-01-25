#include "mainwindow.h"
#include <QVBoxLayout> 
#include <QScrollBar>

namespace timeline {

	MainWindow::MainWindow(QWidget* parent /* = Q_NULLPTR */)
		: QScrollArea(parent)
	{
		setWindowIcon(QIcon(":/images/logo")); 
		resize(800, 150);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); 

		QVBoxLayout* mainLayout = new QVBoxLayout(this);
		mainLayout->setSpacing(0);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mZoomer = new Zoomer(this);
		addScrollBarWidget(mZoomer, Qt::AlignLeft);
		
		mRuler = new Ruler(this);
		setWidget(mRuler);

		connect(mZoomer, &Zoomer::zoomerIn, mRuler, &Ruler::onZoomerIn);
		connect(mZoomer, &Zoomer::zoomerOut, mRuler, &Ruler::onZoomerOut);
	}

}


