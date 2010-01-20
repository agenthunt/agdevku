/*
 * Describe.h
 *
 *  Created on: 18-Dec-2009
 *      Author: shailesh
 */

#ifndef DESCRIBE_H_
#define DESCRIBE_H_
#include "../executor/ExecuteStructures.h"
#include "../global/GlobalStructures.h"
class Describe {
public:
	Describe();
	virtual ~Describe();
	STATUS_CODE execute(ExecuteStructures *executeStructures);
};

#endif /* DESCRIBE_H_ */
