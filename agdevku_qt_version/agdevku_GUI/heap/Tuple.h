/*
 * Tuple.h
 *
 *  Created on: 16-Nov-2009
 *      Author: shailesh
 */

#ifndef TUPLE_H_
#define TUPLE_H_
#include "../catalog/Schema.h"
#include <string>
class Tuple {
public:
	Tuple();
	Tuple(Schema& schema,std::vector<std::string>& values);
	Tuple(Schema& schema,char *record,unsigned recordLen);
	virtual ~Tuple();
	char* prepRecordForInsertion();
	unsigned getLength();
	void getField(int fieldNum,void *fieldData);
	void setField(int fieldNum,const void *fieldData);
	bool fieldMatch(int fieldNum,const void *fieldData);
	bool fieldMatch(std::string columnName,const void *fieldData);
	std::string getFieldInStringFormat(std::string columnName);
	bool fieldMatchByConversion(std::string columnName,std::string value);
	bool lessThanByConversion(std::string columnName,std::string value);
	bool lessThanOrEqualByConversion(std::string columnName,std::string value);
	bool greaterThanByConversion(std::string columnName,std::string value);
	bool greaterThanOrEqualByConversion(std::string columnName,std::string value);
	void setFieldByConversion(std::string columnName,std::string value);
	void getKeyData(std::vector<std::string>& columnList,char*& keyData);
	std::string toString();
private:
	Schema schema_;
	std::vector<std::string> values_;
	char *record_;
	unsigned length_;
	bool fromConstructor_1;
};

#endif /* TUPLE_H_ */
