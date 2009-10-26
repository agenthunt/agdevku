/*
 * ReplacementPolicy.h
 *
 *  Created on: 25-Oct-2009
 *      Author: shailesh
 */

#ifndef REPLACEMENTPOLICY_H_
#define REPLACEMENTPOLICY_H_

class ReplacementPolicy {
public:
	virtual int getFrameTobeReplaced() = 0;
	virtual void increasePriority(int frameNumber) = 0;
	virtual void decreasePriority(int frameNumber) = 0;
	virtual void resetPriority(int frameNumber) = 0;

};

#endif /* REPLACEMENTPOLICY_H_ */
