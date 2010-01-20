/*
 * QueryResultViewHandler.h
 *
 *  Created on: 02-Dec-2009
 *      Author: shailesh
 */

#ifndef QUERYRESULTVIEWHANDLER_H_
#define QUERYRESULTVIEWHANDLER_H_
#include "../global/GlobalStructures.h"
#include <QWidget>
#include <QLayout>
#include <QStackedLayout>
#include <QQueue>
class QueryResultViewHandler {
public:
	QueryResultViewHandler();
	virtual ~QueryResultViewHandler();
	STATUS_CODE handleQuery(QString& queryString,QQueue<QWidget*> *widgetQueue);
	STATUS_CODE buildResultView();
private:
	QWidget *resultWidget;
	int tmpFixError;
};

#endif /* QUERYRESULTVIEWHANDLER_H_ */
