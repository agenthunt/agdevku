/*
 * Tuple.cpp
 *
 *  Created on: 16-Nov-2009
 *      Author: shailesh
 */

#include "Tuple.h"
#include <string.h>
#include "../utils/debug.h"
#include "../utils/util.h"
#include "../global/GlobalDefines.h"
#include <sstream>
#include <iomanip>
Tuple::Tuple() {
	// TODO Auto-generated constructor stub

}

Tuple::Tuple(Schema& schema, std::vector<std::string>& values) {
	values_ = values;
	schema_ = schema;
	length_ = schema_.numOfColumns * sizeof(int);
	for (unsigned i = 0; i < values.size(); i++) {
		length_ = length_ + schema_.fieldLengths.at(i);
	}
	record_ = new char[length_];
	fromConstructor_1 = true;

}

Tuple::Tuple(Schema& schema, char *record, unsigned recordLen) {
	fromConstructor_1 = false;
	schema_ = schema;
	record_ = record;
	length_ = recordLen;
}

Tuple::~Tuple() {
	// TODO Auto-generated destructor stub
	if (fromConstructor_1 == true) {
		delete record_;
	}
}

char* Tuple::prepRecordForInsertion() {
	//first field will be after the number of pointers
	//which will be equal to num of columns

	DEBUG("len="<<length_);
	for (unsigned i = 0, offset = schema_.numOfColumns * sizeof(int); i
			< values_.size(); i++) {
		DEBUG("offset="<<offset);
		memcpy(&record_[sizeof(int) * i], &offset, sizeof(int));
		if (schema_.fieldTypes.at(i) == TYPE_VARCHAR) {
			const char *val = values_[i].c_str();
			memcpy(&record_[offset], val, schema_.fieldLengths.at(i));
			DEBUG("varchar["<<i<<"]="<<(char)record_[offset]);
		} else if (schema_.fieldTypes.at(i) == TYPE_INTEGER) {
			int val = Util::string_to_int(values_.at(i));
			memcpy(&record_[offset], &val, schema_.fieldLengths.at(i));
			DEBUG("val,int="<<val<<","<<(int)record_[offset]);
		}

		offset = offset + schema_.fieldLengths.at(i);
	}

	return record_;
}

unsigned Tuple::getLength() {
	return length_;
}

void Tuple::getField(int fieldNum, void *fieldData) {
	DEBUG("fieldNum="<<fieldNum);
	int cur_field_offset;

	memcpy(&cur_field_offset, &record_[fieldNum * sizeof(int)], sizeof(int));

	DEBUG("cur_field_offset="<<cur_field_offset);
	if (cur_field_offset >= 0 && cur_field_offset <= 8192) {//temp hack to fix a bug
		memcpy(fieldData, &record_[cur_field_offset], schema_.fieldLengths.at(
				fieldNum));
	}
}

std::string Tuple::getFieldInStringFormat(std::string columnName) {
	std::ostringstream mystream;
	int columnNum = schema_.getColumnNum(columnName);
	int colType = schema_.fieldTypes.at(columnNum);
	switch (colType) {
	case TYPE_INTEGER: {
		int recordData;
		getField(columnNum, &recordData);
		mystream << recordData;
		break;
	}
	case TYPE_VARCHAR: {
		char recordData[schema_.fieldLengths.at(columnNum)];
		getField(columnNum, recordData);
		mystream << recordData;
		break;
	}
	default:
		mystream << "COULDNT GET";
		break;
	}
	return mystream.str();
}

bool Tuple::fieldMatch(int fieldNum, const void *fieldData) {
	if (schema_.fieldTypes.at(fieldNum) == TYPE_INTEGER) {
		int recordData;
		getField(fieldNum, &recordData);
		int inputData = *((int*) fieldData);
		if (inputData == recordData) {
			return true;
		} else {
			return false;
		}
	} else if (schema_.fieldTypes.at(fieldNum) == TYPE_VARCHAR) {
		char recordData[schema_.fieldLengths.at(fieldNum)];
		getField(fieldNum, recordData);
		if (strcmp((char*) fieldData, recordData) == 0) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Tuple::fieldMatch(std::string columnName, const void *fieldData) {
	int columnNum = schema_.getColumnNum(columnName);
	return fieldMatch(columnNum, fieldData);
}

bool Tuple::fieldMatchByConversion(std::string columnName, std::string value) {
	int fieldNum = schema_.getColumnNum(columnName);
	if (schema_.fieldTypes.at(fieldNum) == TYPE_INTEGER) {
		int recordData;
		getField(fieldNum, &recordData);
		int inputData = Util::string_to_int(value);
		if (inputData == recordData) {
			return true;
		} else {
			return false;
		}
	} else if (schema_.fieldTypes.at(fieldNum) == TYPE_VARCHAR) {
		char recordData[schema_.fieldLengths.at(fieldNum)];
		getField(fieldNum, recordData);
		if (strcmp(value.c_str(), recordData) == 0) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Tuple::lessThanByConversion(std::string columnName, std::string value) {
	int fieldNum = schema_.getColumnNum(columnName);
	if (schema_.fieldTypes.at(fieldNum) == TYPE_INTEGER) {
		int recordData;
		getField(fieldNum, &recordData);
		int inputData = Util::string_to_int(value);
		if (recordData < inputData) {
			return true;
		} else {
			return false;
		}
	} else if (schema_.fieldTypes.at(fieldNum) == TYPE_VARCHAR) {
		char recordData[schema_.fieldLengths.at(fieldNum)];
		getField(fieldNum, recordData);
		if (strcmp(recordData, value.c_str()) < 0) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Tuple::lessThanOrEqualByConversion(std::string columnName,
		std::string value) {
	int fieldNum = schema_.getColumnNum(columnName);
	if (schema_.fieldTypes.at(fieldNum) == TYPE_INTEGER) {
		int recordData;
		getField(fieldNum, &recordData);
		int inputData = Util::string_to_int(value);
		if (recordData <= inputData) {
			return true;
		} else {
			return false;
		}
	} else if (schema_.fieldTypes.at(fieldNum) == TYPE_VARCHAR) {
		char recordData[schema_.fieldLengths.at(fieldNum)];
		getField(fieldNum, recordData);
		if (strcmp(recordData, value.c_str()) <= 0) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Tuple::greaterThanByConversion(std::string columnName, std::string value) {
	int fieldNum = schema_.getColumnNum(columnName);
	if (schema_.fieldTypes.at(fieldNum) == TYPE_INTEGER) {
		int recordData;
		getField(fieldNum, &recordData);
		int inputData = Util::string_to_int(value);
		if (recordData > inputData) {
			return true;
		} else {
			return false;
		}
	} else if (schema_.fieldTypes.at(fieldNum) == TYPE_VARCHAR) {
		char recordData[schema_.fieldLengths.at(fieldNum)];
		getField(fieldNum, recordData);
		if (strcmp(recordData, value.c_str()) > 0) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Tuple::greaterThanOrEqualByConversion(std::string columnName,
		std::string value) {
	int fieldNum = schema_.getColumnNum(columnName);
	if (schema_.fieldTypes.at(fieldNum) == TYPE_INTEGER) {
		int recordData;
		getField(fieldNum, &recordData);
		int inputData = Util::string_to_int(value);
		if (recordData >= inputData) {
			return true;
		} else {
			return false;
		}
	} else if (schema_.fieldTypes.at(fieldNum) == TYPE_VARCHAR) {
		char recordData[schema_.fieldLengths.at(fieldNum)];
		getField(fieldNum, recordData);
		if (strcmp(recordData, value.c_str()) >= 0) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

void Tuple::setField(int fieldNum, const void *fieldData) {
	DEBUG("setting fieldNum="<<fieldNum);
	int cur_field_offset;
	memcpy(&cur_field_offset, &record_[fieldNum * sizeof(int)], sizeof(int));
	DEBUG("cur_field_offset="<<cur_field_offset);
	memcpy(&record_[cur_field_offset], fieldData, schema_.fieldLengths.at(
			fieldNum));
}

void Tuple::setFieldByConversion(std::string columnName, std::string value) {
	DEBUG("setting fieldNum by conversion="<<columnName);
	int fieldNum = schema_.getColumnNum(columnName);
	if (schema_.fieldTypes.at(fieldNum) == TYPE_INTEGER) {
		int recordData = Util::string_to_int(value);
		setField(fieldNum, &recordData);//can act as setting also
	} else if (schema_.fieldTypes.at(fieldNum) == TYPE_VARCHAR) {
		//char recordData[schema_.fieldLengths.at(fieldNum)];
		const char *recordData = value.c_str();
		setField(fieldNum, recordData);
	}
}

std::string Tuple::toString() {

	std::ostringstream mystream;
	mystream << std::uppercase;
	mystream << std::setfill(' ') << std::setw(20);
	DEBUG("schema_.numOfColumns="<<schema_.numOfColumns);
	for (int i = 0; i < schema_.numOfColumns; i++) {
		DEBUG("inside toString() Tuple");
		int colType = schema_.fieldTypes.at(i);
		switch (colType) {
		case TYPE_INTEGER: {
			int recordData;
			getField(i, &recordData);
			mystream << std::uppercase;
			mystream << std::setfill(' ') << std::setw(20);
			mystream << recordData;
			break;
		}
		case TYPE_VARCHAR: {
			char recordData[schema_.fieldLengths.at(i)];
			getField(i, recordData);
			mystream << std::uppercase;
			mystream << std::setfill(' ') << std::setw(20);
			mystream << recordData;
			break;
		}
		default:
			mystream << "TYPE NOT MATCHED";
			break;
		}
	}
	DEBUG("tuple toString"<<mystream.str());
	return mystream.str();
}

void Tuple::getKeyData(std::vector<std::string>& columnList, char*& keyData) {
	int offset = 0;
	for (int i = 0; i < columnList.size(); i++) {
		int fieldNum = schema_.getColumnNum(columnList[i]);
		if (schema_.fieldTypes.at(fieldNum) == TYPE_INTEGER) {
			int recordData;
			getField(fieldNum, &recordData);
			memcpy(&keyData[offset], &recordData,
					schema_.fieldLengths[fieldNum]);
			offset = offset + schema_.fieldLengths[fieldNum];
		} else if (schema_.fieldTypes.at(fieldNum) == TYPE_VARCHAR) {
			char recordData[schema_.fieldLengths.at(fieldNum)];
			getField(fieldNum, recordData);
			memcpy(&keyData[offset], &recordData,
					schema_.fieldLengths[fieldNum]);
			offset = offset + schema_.fieldLengths[fieldNum];
		}
	}
}
