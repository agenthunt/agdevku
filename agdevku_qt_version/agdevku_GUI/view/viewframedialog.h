#ifndef VIEWFRAMEDIALOG_H
#define VIEWFRAMEDIALOG_H
#include <QVBoxLayout>
#include <QtGui/QDialog>
#include "ui_viewframedialog.h"
#include <QTextEdit>
#include <QSplitter>
#include <string>
#include <QPushButton>
#include "../bufmgr/BufferManager.h"
class ViewFrameDialog : public QDialog
{
    Q_OBJECT

public:
    ViewFrameDialog(int frameNumber,QWidget *parent = 0);

    ~ViewFrameDialog();
    void refreshView();

public slots:
	void handleRefreshButtonClicked();

private:
    Ui::ViewFrameDialogClass ui;
    QVBoxLayout *mainLayout;
    QSplitter *splitter;
    QTextEdit *formattedViewer;
    QTextEdit *hexViewer;
    std::string hexData;
    std::string intelliData;
    BufferManager *bufMgr;
    int pageNumber;
    int frameNumber_;
    QPushButton *refreshButton;
};

#endif // VIEWFRAMEDIALOG_H
