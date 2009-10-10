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

private:
	int pageNumber_;
	bool dirty_;
	int pinCount_;
	char* pageData_;
	unsigned int priority_;
};

#endif /* FRAME_H_ */
