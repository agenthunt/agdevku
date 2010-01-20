#ifndef HEAPVIEWER_H
#define HEAPVIEWER_H

#include <QtGui/QDialog>
#include "ui_heapviewer.h"
#include <QVBoxLayout>
#include "../bufmgr/BufferManager.h"
#include "../heap/HeapFile.h"
#include "mytreewidget.h"
class HeapViewer : public QDialog
{
    Q_OBJECT

public:
    HeapViewer(QString tableName,QWidget *parent = 0);
    ~HeapViewer();
    void initHeapViewer();
private:
    Ui::HeapViewerClass ui;
    QVBoxLayout *mainLayout;
    QString tableName_;
    BufferManager *bufMgr;
    HeapFile *heapFile;
    MyTreeWidget *myTreeWidget;

};

#endif // HEAPVIEWER_H
