#ifndef SELECTVIEW_H
#define SELECTVIEW_H

#include <QtGui/QWidget>
#include "ui_selectview.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include "../executor/Select.h"
#include "../model/SelectModel.h"
class SelectView : public QWidget
{
    Q_OBJECT

public:
    SelectView(Select *select,QWidget *parent = 0);
    ~SelectView();
    void refreshView();

public slots:
	void handleFirstButtonClicked();
	void handleLastButtonClicked();
	void handlePrevButtonClicked();
	void handleNextButtonClicked();

private:
    Ui::SelectViewClass ui;
    QVBoxLayout *mainLayout;
    QHBoxLayout *navigationHelperLayout;
    QPushButton *firstButton;
    QPushButton *lastButton;
    QPushButton *prevButton;
    QPushButton *nextButton;
    QLabel *curPageLabel;
    QTableView *resultSetTableView;
    SelectModel *selectModel;
    Select *select_;
	QLabel *numberOfRowsReturnedMsg;
	QLabel *executionTimeMsg;
	QHBoxLayout *msgLayout;
};

#endif // SELECTVIEW_H
