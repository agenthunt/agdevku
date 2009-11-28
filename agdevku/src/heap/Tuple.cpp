/*
 * Tuple.cpp
 *
 *  Created on: 16-Nov-2009
 *      Author: shailesh
 */

#include "Tuple.h"
#include <string.h>
#include "../utils/debug.h"
#include "../global/GlobalDefines.h"

Tuple::Tuple() {
	// TODO Auto-generated constructor stub

}

Tuple::Tuple(Schema& schema, std::vector<std::string> values) {
	values_ = values;
	schema_ = schema;
	length_ = schema_.numOfColumns * sizeof(int);
	for (unsigned i = 0; i < values.size(); i++) {
		length_ = length_ + schema_.fieldLengths.at(i);
	}
	record_ = new char[length_];

}

Tuple::Tuple(Schema& schema,char *record,unsigned recordLen){
	record_ = record;
	length_ = recordLen;
}

Tuple::~Tuple() {
	// TODO Auto-generated destructor stub
	delete record_;
}

char* Tuple::prepRecordForInsertion() {
	//first field will be after the number of pointers
	//which will be equal to num of columns

	DEBUG("len="<<length_);
	for (unsigned i = 0,offset = schema_.numOfColumns * sizeof(int); i < values_.size(); i++) {
		memcpy(&record_[sizeof(int) * i], &offset, sizeof(int));
		const char *val = values_[i].c_str();
		DEBUG("offset="<<offset);
		memcpy(&record_[offset],val , schema_.fieldLengths.at(i));
		DEBUG("vals["<<i<<"]="<<(char)record_[offset]);
		offset = offset + schema_.fieldLengths.at(i);
	}

	return record_;
}

unsigned Tuple::getLength() {
	return length_;
}


void Tuple::getField(int fieldNum,void *fieldData){
	int cur_field_offset;
	memcpy(&cur_field_offset,&record_[fieldNum * sizeof(int)],sizeof(int));
	memcpy(fieldData,&record_[cur_field_offset],schema_.fieldLengths.at(fieldNum));
}

bool Tuple::fieldMatch(int fieldNum,void *fieldData){
	if(schema_.fieldTypes.at(fieldNum)==TYPE_INTEGER){
		int recordData;
		getField(fieldNum,&recordData);
		int inputData =  *((int*)fieldData);
		return (inputData == recordData);
	}else if(schema_.fieldTypes.at(fieldNum)==TYPE_VARCHAR){
		char *recordData;
		getField(fieldNum,recordData);
		if(strcmp((char*)fieldData,recordData)==0){
			return true;
		}else{
			return false;
		}
	}
	return false;
}
