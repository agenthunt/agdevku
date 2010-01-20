#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"
#include "../mainmenutabwidget.h"
#include <QGridLayout>
#include "createdatabasedialog.h"
#include "opendatabasedialog.h"
#include "initializebufferdialog.h"
#include "diskviewer.h"
#include "../view/heapviewer.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

   void setTitle(QString databaseName);
   void setTitle(std::string databaseName);

private slots:
	void createDatabaseAction();
	void openDatabaseAction();
	void initializeBufferAction();
	void openDiskViewerAction();
	void aboutMenuAction();
	void closeDatabaseAction();
	void toggleCommonDebugAction(bool enabled);
	void toggleHeapVerboseAction(bool enabled);
	void toggleUseIndexAction(bool enabled);
	void toggleBPlustreeDebugAction(bool enabled);
	void runTestsAction();
	void openHeapViewerAction();
	void toggleClientSideSortAction(bool enabled);
	void toggleUseMMAP(bool enabled);

private:
    Ui::MainWindowClass ui;
    QGridLayout *gridLayout;
    MainMenuTabWidget *mainMenuTablWidget;
	CreateDatabaseDialog *createDatabaseDialog;
	OpenDatabaseDialog *openDatabaseDialog;
	InitializeBufferDialog *initializeBufferDialog;
	DiskViewer *diskViewer;
	HeapViewer *heapViewer;
};

#endif // MAINWINDOW_H
