//============================================================================
// Name        : agdevku.cpp
// Author      : shailesh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "../bufmgr/BufferManager.h"
#include "../global/GlobalConstants.h"// this line should be there always
#include "../tests/TestSimpleDiskFileAccessor.cpp"
#include "../tests/TestDiskManager.cpp"
#include "../tests/TestFreePageManager.cpp"
#include "../tests/TestBufferManager.cpp"
#include "../tests/TestLinkedListFreePageManager.cpp"
#include "../tests/TestLRUReplacementPolicy.cpp"
#include "../global/GlobalStructures.h"
#include "../tests/TestHeapFile.cpp"
#include <iostream>
using namespace std;
int main(void) {
	//	BufferManager bufferManager;
	//	bufferManager.initializeBuffer(1);
	//	bufferManager.createDatabase("hello");

	//	TestSimpleDiskFileAccessor testSimpleDiskFileAccessor;
	//	testSimpleDiskFileAccessor.basicTest();
	//
	//	TestDiskManager testDiskManager;
	//	testDiskManager.basicTest();

	//	TestFreePageManager testFreePageManager;
	//	testFreePageManager.isFreePageTest();
	//	testFreePageManager.basicTest();

//		TestBufferManager testBufferManager;
//		testBufferManager.newPageTest();
		//testBufferManager.resizeDatabaseTest();
		//testBufferManager.freePageTest();
		//testBufferManager.createDatabaseTest();


//	TestLinkedListFreePageManager testLinkedListFreePageManager;
//	testLinkedListFreePageManager.basicTest();

//	TestLRUReplacementPolicy testLRUReplacementPolicy;
//	testLRUReplacementPolicy.basicTest();

//	cout<<sizeof(RIDStruct)<<endl;

	TestHeapFile testHeapFile;
	//testHeapFile.insertRecordTest();
	testHeapFile.updateRecordTest();
	return 0;
}
