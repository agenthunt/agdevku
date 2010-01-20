#ifndef BUFFERMENUTABWIDGET_H
#define BUFFERMENUTABWIDGET_H

#include <QtGui/QWidget>
#include <QGridLayout>
#include "ui_buffermenutabwidget.h"
#include "viewframebuffertableview.h"
#include "hitrateview.h"
#include <QVBoxLayout>
#include <QLabel>
class BufferMenuTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    BufferMenuTabWidget(QWidget *parent = 0);
    ~BufferMenuTabWidget();

public :
    void refreshTabs();
private slots:
	void currentTabChanged(int tabNumber);
	void handleReadPagePushButton();
	void handleWritePagePushButton();
private:
    Ui::BufferMenuTabWidgetClass ui;
    ViewFrameBufferTableView *viewFrameBufferTableView;
    HitRateView *hitRateView;
    QVBoxLayout *hitRateTabLayout;
    QGridLayout *gridLayout;
	BufferManager *bufMgr;

};

#endif // BUFFERMENUTABWIDGET_H
