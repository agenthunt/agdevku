/*
 * PostFixEvaluator.h
 *
 *  Created on: 18-Dec-2009
 *      Author: shailesh
 */

#ifndef POSTFIXEVALUATOR_H_
#define POSTFIXEVALUATOR_H_
#include <vector>
#include <stack>
#include "../heap/Tuple.h"
#include "../executor/ExprInfo.h"
class PostFixEvaluator {
public:
	PostFixEvaluator(Tuple& tuple);
	virtual ~PostFixEvaluator();
	bool evaluate(std::vector<ExprInfo> exprList);
	std::stack<ExprInfo> myStack;
	ExprInfo eval(ExprInfo exp1,ExprInfo exp2,ExprInfo op);
	Tuple tuple_;
};

#endif /* POSTFIXEVALUATOR_H_ */
