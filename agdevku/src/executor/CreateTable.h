/*
 * CreateTable.h
 *
 *  Created on: 16-Nov-2009
 *      Author: shailesh
 */

#ifndef CREATETABLE_H_
#define CREATETABLE_H_
#include "../catalog/Schema.h"
class CreateTable {
public:
	CreateTable();
	virtual ~CreateTable();
	/**
	 * create table, add create a headerPage and add into sysTable and
	 * sysCols
	 */
	void execute();
private:
	Schema schema_;

};

#endif /* CREATETABLE_H_ */
