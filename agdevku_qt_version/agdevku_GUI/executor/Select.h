/*
 * Select.h
 *
 *  Created on: 03-Dec-2009
 *      Author: shailesh
 */

#ifndef SELECT_H_
#define SELECT_H_
#include "../executor/ExecuteStructures.h"
#include "../global/GlobalStructures.h"
#include "../heap/TableScan.h"
#include "../heap/Tuple.h"
#include "../executor/ExprInfo.h"
#include "../utils/timer_util.h"
#include <vector>
#include <ctime>
class Select {
public:
	Select();
	virtual ~Select();
	STATUS_CODE execute(ExecuteStructures *executeStructures);
	void loadFirstPage();
	void loadNextPage();
	void loadPrevPage();
	void loadLastPage();
	std::vector<std::string> resultSetColumnList_;
	std::vector<ExprInfo> whereConditionExprList_;
	std::vector<Tuple> resultSet;
	bool allColumns_;
	Schema schema;
	HeapFile *heapFile;
	time_t start, end;
	double diff;
	int totalNumOfRecords;
	TimerUtil timerUtil;
	std::vector<RIDStruct> ridList;
	std::vector<std::string> keyList;

private:
	TableScan *tableScan;
	bool tableScanOpened;

};

#endif /* SELECT_H_ */
