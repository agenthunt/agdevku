//============================================================================
// Name        : agdevku.cpp
// Author      : shailesh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================



#include "../bufmgr/BufferManager.h"
#include "../global/GlobalConstants.h"
#include "../tests/TestSimpleDiskFileAccessor.cpp"
#include "../tests/TestDiskManager.cpp"
#include <iostream>
using namespace std;
int main(void) {
//	BufferManager bufferManager;
//	bufferManager.initializeBuffer(1);
//	bufferManager.createDatabase("hello");
	TestSimpleDiskFileAccessor testSimpleDiskFileAccessor;
	testSimpleDiskFileAccessor.basicTest();

	TestDiskManager testDiskManager;
	testDiskManager.createDatabaseTest();

	return 0;
}
