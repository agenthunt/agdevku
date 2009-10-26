/*
 * LRUReplacementPolicy.cpp
 *
 *  Created on: 25-Oct-2009
 *      Author: shailesh
 */

#include <climits>
#include "LRUReplacementPolicy.h"
#include "BufferManager.h"
#include "Frame.h"
LRUReplacementPolicy::LRUReplacementPolicy() {
	// TODO Auto-generated constructor stub


}

LRUReplacementPolicy::~LRUReplacementPolicy() {
	// TODO Auto-generated destructor stub
}

int LRUReplacementPolicy::getFrameTobeReplaced() {
	BufferManager *bufferManager = BufferManager::getInstance();
	int frameToBeReplaced = -1;
	int minPriority = ULONG_MAX;
	for (int i = 0; i < bufferManager->numOfFrames_; i++) {
		if (bufferManager->bufferPool_[i]->pinCount_ == 0) {
			if(bufferManager->bufferPool_[i]->priority_ < minPriority){
				minPriority = bufferManager->bufferPool_[i]->priority_;
				frameToBeReplaced = i;
			}

		}
	}
	return frameToBeReplaced;
}

void LRUReplacementPolicy::increasePriority(int frameNumber) {
	BufferManager *bufferManager = BufferManager::getInstance();
	bufferManager->bufferPool_[frameNumber]->priority_++;
}

void LRUReplacementPolicy::decreasePriority(int frameNumber) {
	BufferManager *bufferManager = BufferManager::getInstance();
	bufferManager->bufferPool_[frameNumber]->priority_--;
}

void LRUReplacementPolicy::resetPriority(int frameNumber){
	BufferManager *bufferManager = BufferManager::getInstance();
	bufferManager->bufferPool_[frameNumber]->priority_ = 0;
}

