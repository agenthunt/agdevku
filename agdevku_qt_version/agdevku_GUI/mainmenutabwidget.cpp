#include "mainmenutabwidget.h"

MainMenuTabWidget::MainMenuTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
	ui.setupUi(this);
	gridLayout = new QGridLayout();
	bufferMenuTabWidget = new BufferMenuTabWidget();
	gridLayout->addWidget(bufferMenuTabWidget);
	ui.bufMgrTab->setLayout(gridLayout);

	sqlGridLayout = new QGridLayout();
	sqlMenuWidget = new SQLMenuWidget();
	sqlGridLayout->addWidget(sqlMenuWidget);
	ui.sqlTab->setLayout(sqlGridLayout);
}

MainMenuTabWidget::~MainMenuTabWidget()
{
	delete gridLayout;
	delete bufferMenuTabWidget;
}
