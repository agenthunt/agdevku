/*
 * LRUReplacementPolicy.h
 *
 *  Created on: 25-Oct-2009
 *      Author: shailesh
 */

#ifndef LRUREPLACEMENTPOLICY_H_
#define LRUREPLACEMENTPOLICY_H_
#include "ReplacementPolicy.h"
class BufferManager;

class LRUReplacementPolicy:public ReplacementPolicy {
public:
	LRUReplacementPolicy();
	virtual ~LRUReplacementPolicy();
	virtual int getFrameTobeReplaced();
	virtual void increasePriority(int frameNumber);
	virtual void decreasePriority(int frameNumber);
	virtual void resetPriority(int frameNumber);


};

#endif /* LRUREPLACEMENTPOLICY_H_ */
