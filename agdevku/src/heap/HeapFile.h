/*
 * HeapFile.h
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#ifndef HEAPFILE_H_
#define HEAPFILE_H_
#include "../global/GlobalStructures.h"
#include "DirectoryHeaderPage.h"
/**
 * All tables are stored in Heapfiles. Heap file is a logical file and
 * physically it is just a bunch of pages . We follow the directory chain
 * model.
 */
class HeapFile {
public:
	HeapFile(const char *fileName);
	HeapFile(int dirHeaderPageNumber);
	virtual ~HeapFile();
	//void createFile(const char *fileName);//does not make sense to have
	//this method, constructor suffices the purpose
	void deleteFile();
	STATUS_CODE insertRecord(char *record,unsigned recordLen,RIDStruct& rid);
	void deleteRecord(const RIDStruct& rid);
	STATUS_CODE updateRecord(const RIDStruct rid,char *record,unsigned recordLen);
	void getRecord(const RIDStruct& rid,char *record,unsigned& recordLen);
	int getNextRecordNumber();//used in generating table IDs for sysTable....
private:
	DirectoryHeaderPage *dirHeaderPage_;
};

#endif /* HEAPFILE_H_ */
