/*
 * CreateTable.h
 *
 *  Created on: 16-Nov-2009
 *      Author: shailesh
 */

#ifndef CREATETABLE_H_
#define CREATETABLE_H_
#include "../catalog/Schema.h"
#include "../global/GlobalStructures.h"
#include "../executor/ExecuteStructures.h"
class CreateTable {
public:
	CreateTable();
	virtual ~CreateTable();
	/**
	 * create table, add create a headerPage and add into sysTable and
	 * sysCols
	 */
	STATUS_CODE execute(ExecuteStructures *executeStructures);
private:
	Schema schema_;

};

#endif /* CREATETABLE_H_ */
