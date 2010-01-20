#ifndef MAINMENUTABWIDGET_H
#define MAINMENUTABWIDGET_H

#include <QtGui/QWidget>
#include "ui_mainmenutabwidget.h"
#include <QGridLayout>
#include "../view/buffermenutabwidget.h"
#include "../view/sqlmenuwidget.h"

class MainMenuTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    MainMenuTabWidget(QWidget *parent = 0);
    ~MainMenuTabWidget();
    BufferMenuTabWidget *bufferMenuTabWidget;
    SQLMenuWidget *sqlMenuWidget;
private:

    Ui::MainMenuWidgetTab ui;
    QGridLayout *gridLayout;

    QGridLayout *sqlGridLayout;
};

#endif // MAINMENUTABWIDGET_H
