//============================================================================
// Name        : agdevku.cpp
// Author      : shailesh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================



#include "../bufmgr/BufferManager.h"
#include "../global/GlobalConstants.h"
int main(void) {
	BufferManager bufferManager;
	bufferManager.initializeBuffer(1);
	bufferManager.createDatabase("hello");
	return 0;
}
