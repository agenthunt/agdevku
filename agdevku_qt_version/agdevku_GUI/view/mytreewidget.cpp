#include "mytreewidget.h"
#include <QPainter>
#include "../utils/debug.h"
MyTreeWidget::MyTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
	ui.setupUi(this);
	DEBUG("in my tree widget");

}

MyTreeWidget::~MyTreeWidget()
{

}
void MyTreeWidget::paintEvent(QPaintEngine *engine){
	DEBUG("in mytree widget paintEvent");
}

void MyTreeWidget::drawTree(QPainter *painter,const QRegion& region){
	DEBUG("in mytree widget drawTree");
	painter->drawText(10,10,"HELLO");
	repaint();

}
