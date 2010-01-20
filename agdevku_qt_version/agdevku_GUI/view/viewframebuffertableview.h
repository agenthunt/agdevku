#ifndef VIEWFRAMEBUFFERTABLEVIEW_H
#define VIEWFRAMEBUFFERTABLEVIEW_H

#include <QtGui/QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include "ui_viewframebuffertableview.h"
#include "../model/FrameBufferModel.h"
#include "../model/SplitFrameBufferModel.h"
class ViewFrameBufferTableView : public QWidget
{
    Q_OBJECT

public:
    ViewFrameBufferTableView(QWidget *parent = 0);
    ~ViewFrameBufferTableView();
    void refreshView();

public slots:
	void handleFirstButtonClicked();
	void handleLastButtonClicked();
	void handlePrevButtonClicked();
	void handleNextButtonClicked();
	void handleRefreshButtonClicked();

private:
    Ui::ViewFrameBufferTableViewClass ui;
    //FrameBufferModel *frameBufferModel;
    QList<QPushButton*> viewFrameButtonList;
    QList<QPushButton*> commitBufferButtonList;
    QButtonGroup *viewFrameButtonGroup;
    QButtonGroup *commitBufferButtonGroup;

    QVBoxLayout *mainLayout;
    QHBoxLayout *navigationHelperLayout;
    QPushButton *firstButton;
    QPushButton *lastButton;
    QPushButton *prevButton;
    QPushButton *nextButton;
    QPushButton *refreshButton;
    QLabel *curPageLabel;
    QTableView *frameBufferTableView;
    SplitFrameBufferModel *frameBufferModel;

private slots:
	void handleViewFrameClick(int id);
	void handleCommitBufferClick(int id);

};

#endif // VIEWFRAMEBUFFERTABLEVIEW_H
