/*
 * TestPostFixEvaluator.h
 *
 *  Created on: 22-Dec-2009
 *      Author: shailesh
 */

#ifndef TESTPOSTFIXEVALUATOR_H_
#define TESTPOSTFIXEVALUATOR_H_
#include <iostream>
#include "../bufmgr/BufferManager.h"
#include "../executor/ExprInfo.h"
#include "../utils/PostFixEvaluator.h"
#include "../catalog/CatalogUtil.h"
#include "../heap/Tuple.h"
#include "../utils/debug.h"
#include "../utils/ErrorLookupTable.h"
using namespace std;
class TestPostFixEvaluator {

public:
	string testName;
	string methodName;
	BufferManager *bufferManager;
	int error;
	TestPostFixEvaluator() {
		methodName = "constructor";
		testName = "TestPostFixEvaluator";
		bufferManager = BufferManager::getInstance();
		bufferManager->openDatabase("HELLO");
	}

	~TestPostFixEvaluator() {
		bufferManager->closeDatabase();
	}

	STATUS_CODE testEvaluate() {
		methodName = "testEvaluate";
		DEBUG(methodName)
		Schema schema;
		CatalogUtil catalogUtil;
		int error = catalogUtil.getSchema("FIRST",schema);
		DEBUG("error="<<ErrorLookupTable::lookup(error));
		std::vector<std::string> values;
		std::string str = "100";
		values.push_back(str);
		DEBUG("values at0"<<values.at(0));
		Tuple tuple(schema,values);
		tuple.prepRecordForInsertion();
		PostFixEvaluator postFix(tuple);
		ExprInfo expInfo1(ExprInfo::IDENTIFIER_TYPE,"COL");
		ExprInfo expInfo2(ExprInfo::LITERAL_TYPE,"100");
		ExprInfo expInfo3(ExprInfo::OPERATOR_TYPE,"=");
		std::vector<ExprInfo> exprList;
		exprList.push_back(expInfo1);
		exprList.push_back(expInfo2);
		exprList.push_back(expInfo3);
		bool res = postFix.evaluate(exprList);
		DEBUG("Res="<<res);
		return SUCCESS;
	}

};

#endif /* TESTPOSTFIXEVALUATOR_H_ */
