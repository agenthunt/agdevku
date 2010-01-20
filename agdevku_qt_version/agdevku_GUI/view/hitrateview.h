#ifndef HITRATEVIEW_H
#define HITRATEVIEW_H

#include <QtGui/QWidget>
#include "ui_hitrateview.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include "../bufmgr/BufferManager.h"
class HitRateView : public QWidget
{
    Q_OBJECT

public:
    HitRateView(QWidget *parent = 0);
    ~HitRateView();
    void refreshView();

private:
    Ui::HitRateViewClass ui;
    QVBoxLayout *mainLayout;
    QLineEdit *hitRateText;
    BufferManager *bufMgr;
	QLabel *hitRateLabel;
};

#endif // HITRATEVIEW_H
