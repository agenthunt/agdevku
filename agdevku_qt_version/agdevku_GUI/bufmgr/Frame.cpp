/*
 * Frame.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "Frame.h"

Frame::Frame(int pageSize) {
	// TODO Auto-generated constructor stub
	pageNumber_ = -1;
	dirty_ = false;
	pinCount_ = -1; //indicates free frame
	priority_ = 0;
	pageData_ = new char[pageSize];
}

Frame::~Frame() {
	// TODO Auto-generated destructor stub
	delete[] pageData_;
}



