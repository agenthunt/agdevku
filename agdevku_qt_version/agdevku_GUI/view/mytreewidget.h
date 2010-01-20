#ifndef MYTREEWIDGET_H
#define MYTREEWIDGET_H

#include <QtGui/QTreeWidget>
#include "ui_mytreewidget.h"

class MyTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    MyTreeWidget(QWidget *parent = 0);
    ~MyTreeWidget();
    void drawTree(QPainter *painter,const QRegion& region);
    void paintEvent(QPaintEngine *engine);

private:
    Ui::MyTreeWidgetClass ui;
};

#endif // MYTREEWIDGET_H
