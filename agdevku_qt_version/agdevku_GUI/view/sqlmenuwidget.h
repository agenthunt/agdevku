#ifndef SQLMENUWIDGET_H
#define SQLMENUWIDGET_H

#include <QtGui/QWidget>
#include "ui_sqlmenuwidget.h"
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QStackedLayout>
#include <QQueue>
#include "../global/StatusCodes.h"
#include "QueryResultViewHandler.h"
class SQLMenuWidget : public QWidget
{
    Q_OBJECT

public:
    SQLMenuWidget(QWidget *parent = 0);
    ~SQLMenuWidget();

    //void keyPressEvent(QKeyEvent *keyEvent);
    void keyReleaseEvent(QKeyEvent *keyEvent);
	void executeSQLFile(QString fileName);


private slots:
	STATUS_CODE handleQueryEnter();
	void handleClearButton();
	void handleImportButton();

private:
    Ui::SQLMenuWidgetClass ui;
    QTextEdit *queryEdit;
    QLabel *queryLabel;
    QPushButton *enterButton;
    QPushButton *clearButton;
    QPushButton *importButton;
    QHBoxLayout *queryLayout;
    QVBoxLayout *buttonLayout;
    QVBoxLayout *mainLayout;
    QueryResultViewHandler *queryResultViewHanlder;
    QLayout *resultLayout;
    QStackedLayout *stackedLayout;
    QWidgetList *widgetList;
    QLabel *dummy;
    QQueue<QWidget*> *widgetQueue;
};

#endif // SQLMENUWIDGET_H
