/*
 * Tuple.cpp
 *
 *  Created on: 16-Nov-2009
 *      Author: shailesh
 */

#include "Tuple.h"
#include <string.h>
Tuple::Tuple() {
	// TODO Auto-generated constructor stub

}

Tuple::Tuple(Schema& schema, std::string values[]) {
	values_ = values;
	schema_ = schema;
	length_ = schema.numOfColumns * sizeof(int) + schema.recSize;
	record_ = new char[length_];

}

Tuple::~Tuple() {
	// TODO Auto-generated destructor stub
	delete record_;
}

char* Tuple::prepRecordForInsertion() {
	int val = (schema_.numOfColumns * sizeof(int));
	memcpy(record_, &val, schema_.numOfColumns * sizeof(int));
	int offset = schema_.numOfColumns * sizeof(int);
	for (unsigned i = 1; i < values_->length(); i++) {
		memcpy(&record_[offset], values_->c_str(), schema_.fieldLengths.at(i));
		offset = offset + schema_.fieldLengths.at(i);
	}
	return record_;
}

int Tuple::getLength(){
	return length_;
}
