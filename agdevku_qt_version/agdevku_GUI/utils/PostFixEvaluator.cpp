/*
 * PostFixEvaluator.cpp
 *
 *  Created on: 18-Dec-2009
 *      Author: shailesh
 */

#include "PostFixEvaluator.h"
#include "../utils/debug.h"

PostFixEvaluator::PostFixEvaluator(Tuple& tuple) {
	// TODO Auto-generated constructor stub
	tuple_ = tuple;
}

PostFixEvaluator::~PostFixEvaluator() {
	// TODO Auto-generated destructor stub
}

bool PostFixEvaluator::evaluate(std::vector<ExprInfo> exprList) {
	DEBUG("inside postfix evaluate");
	for (unsigned i = 0; i < exprList.size(); i++) {
		if (exprList[i].type_ == ExprInfo::IDENTIFIER_TYPE || exprList[i].type_
				== ExprInfo::LITERAL_TYPE) {
			myStack.push(exprList[i]);
		} else {
			ExprInfo op = exprList[i];
			DEBUG("OPERATOR="<<op.operatorValue);
			ExprInfo exp1 = myStack.top();
			myStack.pop();
			ExprInfo exp2 = myStack.top();
			myStack.pop();
			ExprInfo res = eval(exp1, exp2, op);
			myStack.push(res);
		}
	}
	return myStack.top().res;
}

ExprInfo PostFixEvaluator::eval(ExprInfo exp1, ExprInfo exp2, ExprInfo op) {
	ExprInfo res;
	res.res = false;
	res.type_ = ExprInfo::RESULT_TYPE;
	DEBUG("OPERATOR="<<op.operatorValue);
	if (op.operatorValue == "=") {
		if (exp1.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.identifierValue<<",exp2="<<exp2.literalValue);
			if (tuple_.fieldMatchByConversion(exp1.identifierValue,
					exp2.literalValue)==true) {
				res.res = true;
			}
		} else if (exp2.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.literalValue<<",exp2="<<exp2.identifierValue);
			if (tuple_.fieldMatchByConversion(exp2.identifierValue,
					exp1.literalValue)==true) {
					res.res = true;
			}
		} else if (exp1.type_ == ExprInfo::RESULT_TYPE) {
			if(exp1.res == exp2.res){
				res.res = true;
			}
		}
	}else 	if (op.operatorValue == "<") {
		if (exp1.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.identifierValue<<",exp2="<<exp2.literalValue);
			if (tuple_.lessThanByConversion(exp1.identifierValue,
					exp2.literalValue)==true) {
				res.res = true;
			}
		} else if (exp2.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.literalValue<<",exp2="<<exp2.identifierValue);
			if (tuple_.lessThanByConversion(exp2.identifierValue,
					exp1.literalValue)==true) {
					res.res = true;
			}
		} else if (exp1.type_ == ExprInfo::RESULT_TYPE) {
			if(exp1.res == exp2.res){
				res.res = true;
			}
		}
	}else 	if (op.operatorValue == "<=") {
		if (exp1.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.identifierValue<<",exp2="<<exp2.literalValue);
			if (tuple_.lessThanOrEqualByConversion(exp1.identifierValue,
					exp2.literalValue)==true) {
				res.res = true;
			}
		} else if (exp2.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.literalValue<<",exp2="<<exp2.identifierValue);
			if (tuple_.lessThanOrEqualByConversion(exp2.identifierValue,
					exp1.literalValue)==true) {
					res.res = true;
			}
		} else if (exp1.type_ == ExprInfo::RESULT_TYPE) {
			if(exp1.res == exp2.res){
				res.res = true;
			}
		}
	}else 	if (op.operatorValue == ">=") {
		if (exp1.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.identifierValue<<",exp2="<<exp2.literalValue);
			if (tuple_.greaterThanOrEqualByConversion(exp1.identifierValue,
					exp2.literalValue)==true) {
				res.res = true;
			}
		} else if (exp2.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.literalValue<<",exp2="<<exp2.identifierValue);
			if (tuple_.greaterThanOrEqualByConversion(exp2.identifierValue,
					exp1.literalValue)==true) {
					res.res = true;
			}
		} else if (exp1.type_ == ExprInfo::RESULT_TYPE) {
			if(exp1.res == exp2.res){
				res.res = true;
			}
		}
	}else 	if (op.operatorValue == ">") {
		if (exp1.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.identifierValue<<",exp2="<<exp2.literalValue);
			if (tuple_.greaterThanByConversion(exp1.identifierValue,
					exp2.literalValue)==true) {
				res.res = true;
			}
		} else if (exp2.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.literalValue<<",exp2="<<exp2.identifierValue);
			if (tuple_.greaterThanByConversion(exp2.identifierValue,
					exp1.literalValue)==true) {
					res.res = true;
			}
		} else if (exp1.type_ == ExprInfo::RESULT_TYPE) {
			if(exp1.res == exp2.res){
				res.res = true;
			}
		}
	}else 	if (op.operatorValue == "AND") {
		if (exp1.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.identifierValue<<",exp2="<<exp2.literalValue);
			if (tuple_.greaterThanByConversion(exp1.identifierValue,
					exp2.literalValue)==true) {
				res.res = true;
			}
		} else if (exp2.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.literalValue<<",exp2="<<exp2.identifierValue);
			if (tuple_.greaterThanByConversion(exp2.identifierValue,
					exp1.literalValue)==true) {
					res.res = true;
			}
		} else if (exp1.type_ == ExprInfo::RESULT_TYPE) {
			if(exp1.res == true && exp2.res == true){
				res.res = true;
			}
		}
	}else 	if (op.operatorValue == "OR") {
		if (exp1.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.identifierValue<<",exp2="<<exp2.literalValue);
			if (tuple_.greaterThanByConversion(exp1.identifierValue,
					exp2.literalValue)==true) {
				res.res = true;
			}
		} else if (exp2.type_ == ExprInfo::IDENTIFIER_TYPE) {
			DEBUG("exp1="<<exp1.literalValue<<",exp2="<<exp2.identifierValue);
			if (tuple_.greaterThanByConversion(exp2.identifierValue,
					exp1.literalValue)==true) {
					res.res = true;
			}
		} else if (exp1.type_ == ExprInfo::RESULT_TYPE) {
			if(exp1.res == true || exp2.res == true){
				res.res = true;
			}
		}
	}
	return res;
}
