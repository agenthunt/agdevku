#include "mainmenutabwidget.h"
#include "view/mainwindow.h"
#include "view/createdatabasedialog.h"
#include <QtGui>
#include <QApplication>
#include <qvalidator.h>
#include "../tests/TestBPlusTree.h"
#include "../tests/TestPostFixEvaluator.cpp"
#include "../utils/util.h"
#include <vector>
MainWindow *mainWindow;
int main(int argc, char *argv[]) {
	vector<std::string> vec = Util::split("sdf", "#");
	cout << "hello" << vec.size();
	cout <<"hello"<<endl;
	QApplication a(argc, argv);
	//MainMenuTabWidget w;
	//w.show();

	mainWindow = new MainWindow();
	mainWindow->show();
	//    CreateDatabaseDialog cr;
	//    cr.show();
	//	TestBPlusTree testBPlusTree;
	//	testBPlusTree.BPlusTreeInsertTest();
//	TestPostFixEvaluator testPostFixEvaluator;
//	testPostFixEvaluator.testEvaluate();

	return a.exec();
}
