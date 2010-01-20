/*
 * ExprInfo.cpp
 *
 *  Created on: 18-Dec-2009
 *      Author: shailesh
 */

#include "ExprInfo.h"

ExprInfo::ExprInfo(){

}


ExprInfo::ExprInfo(int type,std::string value) {
	// TODO Auto-generated constructor stub
	type_ = type;
	if(type == IDENTIFIER_TYPE){
		identifierValue = value;
	}else if(type == LITERAL_TYPE){
		literalValue = value;
	}else if(type == OPERATOR_TYPE){
		operatorValue = value;
	}
}

ExprInfo::~ExprInfo() {
	// TODO Auto-generated destructor stub
}
