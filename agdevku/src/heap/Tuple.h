/*
 * Tuple.h
 *
 *  Created on: 16-Nov-2009
 *      Author: shailesh
 */

#ifndef TUPLE_H_
#define TUPLE_H_
#include "../catalog/Schema.h"

class Tuple {
public:
	Tuple();
	Tuple(Schema& schema,std::vector<std::string> values);
	Tuple(Schema& schema,char *record,unsigned recordLen);
	virtual ~Tuple();
	char* prepRecordForInsertion();
	unsigned getLength();
	void getField(int fieldNum,void *fieldData);
	bool fieldMatch(int fieldNum,void *fieldData);
private:
	Schema schema_;
	std::vector<std::string> values_;
	char *record_;
	unsigned length_;
};

#endif /* TUPLE_H_ */
