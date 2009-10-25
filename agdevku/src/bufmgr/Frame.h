/*
 * Frame.h
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#ifndef FRAME_H_
#define FRAME_H_

class Frame {
public:
	Frame(int pageSize);
	virtual ~Frame();
//	bool isDirty();
//	char *getPageData();
//	int getPageNumber();
//	int getPinCount();
//	unsigned int getPriority();
//	void setDirty(bool dirty_);
//	void setPageNumber(int pageNumber_);
//	void setPinCount(int pinCount_);
//	void setPriority(unsigned int priority_);
//	void incrementPinCount();
//	void decrementPinCount();
private:
	int pageNumber_;
	bool dirty_;
	int pinCount_;
	char* pageData_;
	unsigned int priority_;
	friend class BufferManager;
};

#endif /* FRAME_H_ */
