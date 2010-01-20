/*
 * ExprInfo.h
 *
 *  Created on: 18-Dec-2009
 *      Author: shailesh
 */

#ifndef EXPRINFO_H_
#define EXPRINFO_H_
#include <string>
class ExprInfo {
public:
	ExprInfo();
	ExprInfo(int type,std::string value);
	~ExprInfo();
	int type_;//can be identifier, literal or operator
	static const int LITERAL_TYPE = 0; //values in expression
	static const int IDENTIFIER_TYPE = 1;//can be column names
	static const int OPERATOR_TYPE = 2;// =, or < or >
	static const int RESULT_TYPE = 3;// intermediate and end results
	std::string identifierValue;
	std::string literalValue;
	std::string operatorValue;
	bool res;
};

#endif /* EXPRINFO_H_ */
