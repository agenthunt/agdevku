/*
 * HeapFile.h
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#ifndef HEAPFILE_H_
#define HEAPFILE_H_
#include "../global/GlobalStructures.h"
/**
 * All tables are stored in Heapfiles. Heap file is a logical file and
 * physically it is just a bunch of pages . We follow the directory chain
 * model.
 */
class HeapFile {
public:
	HeapFile(const char *fileName);
	virtual ~HeapFile();
	//void createFile(const char *fileName);//does not make sense to have
	//this method, constructor suffices the purpose
	void deleteFile();
	RID insertRecord(char *record,unsigned recordLen);
	bool deleteRecord(const RID& rid);
	bool updateRecord(const RID& rid,char *record,unsigned recordLen);
	void getRecord(const RID& rid,char *record,unsigned& recordLen);
};

#endif /* HEAPFILE_H_ */
