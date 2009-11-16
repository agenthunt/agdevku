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
	Tuple(Schema& schema,std::string values[]);
	virtual ~Tuple();
	char* prepRecordForInsertion();
	int getLength();
private:
	Schema schema_;
	std::string *values_;
	char *record_;
	int length_;
};

#endif /* TUPLE_H_ */
