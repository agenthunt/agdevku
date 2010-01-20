/*
 * HeapFile.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "HeapFile.h"
#include "../bufmgr/BufferManager.h"
#include "DirectoryPage.h"
#include "DataPage.h"
#include "../global/ExternDefsOfGlobalConstants.h"
#include <string.h>
#include "../utils/debug.h"
#include "../catalog/CatalogUtil.h"
#include "../global/ExternDefOfGlobalVariables.h"
HeapFile::HeapFile(const char *fileName) {
	// TODO Auto-generated constructor stub
	//find out header page number based on fileName given
	CatalogUtil catalogUtil;
	int dirHeaderPageNumber;
	int error = catalogUtil.getHeaderPageNumber(fileName, dirHeaderPageNumber);
	if (SUCCESS != error) {
		constructorErrorStatus = error;
	}
	DEBUG ("DIR HEADER PAGE NUM="<<dirHeaderPageNumber);
	initHeapFile(dirHeaderPageNumber);
}

HeapFile::HeapFile(int dirHeaderPageNumber) {
	initHeapFile(dirHeaderPageNumber);
}

HeapFile::~HeapFile() {
	// TODO Auto-generated destructor stub
	DEBUG("DIR HEADER PAGE NUM="<< dirHeaderPage_->getPageNumber());
	delete dirHeaderPage_;
}

void HeapFile::initHeapFile(int dirHeaderPageNumber) {
	//	BufferManager *bufMgr = BufferManager::getInstance();
	//		char *pageData;
	//		bufMgr->pinAndGetPage(dirHeaderPageNumber, pageData);
	//		dirHeaderPage_ = new DirectoryHeaderPage(pageData);
	//		bufMgr->unPinPage(dirHeaderPageNumber, false);
	dirHeaderPage_ = new DirectoryHeaderPage(dirHeaderPageNumber);
}

STATUS_CODE HeapFile::insertRecord(char *record, unsigned recordLen,
		RIDStruct& rid) {
	int firstDirPageNumber = dirHeaderPage_->getFirstDirectoryPageNumber();
	//when directory header page is created, first dir page is set to -1
	//It indicates , there are no directory pages
	if (firstDirPageNumber == -1) {
		dirHeaderPage_->createFirstDirectoryPage();
		firstDirPageNumber = dirHeaderPage_->getFirstDirectoryPageNumber();
	}

	for (int i = firstDirPageNumber; i != -1;) {
		DirectoryPage dirPage(i);
		int numOfDEs = dirPage.getNumberOfDirectoryEntries();

		for (int j = 0; j < numOfDEs; j++) {
			DirectoryEntry dirEntry;
			dirPage.getDirectoryEntry(j, dirEntry);
			if (dirEntry.getFreeSpace() >= recordLen) {
				rid.pageNumber = dirEntry.getPageNumber();
				DataPage dataPage(rid.pageNumber);
				rid.slotNumber = dataPage.insertRecord(record, recordLen);//after inserting update freespace
				dirPage.updateFreeSpace(j, dataPage.getContFreeSpace());//update other stuff also
				dirHeaderPage_->updateNumOfRecords(
						dirHeaderPage_->getNumberOfRecords() + 1);
				return SUCCESS;
			}
		}
		/*no directory entry i.e page exists to which  has enough free space
		 * to fit the record, therefore create a new page and add a directory entry
		 */
		if (numOfDEs < dirHeaderPage_->getMaxNumberOfDirectoryEntries()) {
			//create a new DataPage
			DataPage newDataPage;
			newDataPage.createDataPage(dirHeaderPage_->getPageNumber(),
					dirPage.getPageNumber());
			DirectoryEntry newDirEntry;
			dirPage.addDirectoryEntry(newDataPage.getPageNumber(),
					newDataPage.getContFreeSpace(), newDirEntry);
			DEBUG("numOfDirs"<<dirPage.getNumberOfDirectoryEntries());
			DEBUG("newDirEntry"<<newDirEntry.toString());
			newDataPage.setDirectoryEntryNumber(
					newDirEntry.getDirectoryEntryNumber());
			rid.pageNumber = newDataPage.getPageNumber();
			rid.slotNumber = newDataPage.insertRecord(record, recordLen);//after inserting update freespace
			//since directoryEntry number starts from one and offset setting should be zero right?
//			dirPage.updateFreeSpace(newDirEntry.directoryEntry_.entryNumber,
//					newDataPage.getContFreeSpace());
			dirPage.updateFreeSpace(newDirEntry.directoryEntry_.entryNumber-1,
								newDataPage.getContFreeSpace());
			dirHeaderPage_->updateNumOfRecords(
					dirHeaderPage_->getNumberOfRecords() + 1);
			return SUCCESS;
		} else if (dirPage.getNextDirectoryPageNumber() == -1) {
			/*
			 * number of dirEntries in the directory page has reached the max num of directory entries
			 * therefore, create a new directory page and add a new directory entry to it
			 */
			DirectoryPage newDirPage;
			newDirPage.createDirectoryPage();
			DataPage newDataPage;
			newDataPage.createDataPage(dirHeaderPage_->getPageNumber(),
					newDirPage.getPageNumber());
			DirectoryEntry newDirEntry;
			newDirPage.addDirectoryEntry(newDataPage.getPageNumber(),
					newDataPage.getContFreeSpace(), newDirEntry);
			newDataPage.setDirectoryEntryNumber(
					newDirEntry.getDirectoryEntryNumber());
			rid.pageNumber = newDataPage.getPageNumber();
			rid.slotNumber = newDataPage.insertRecord(record, recordLen);//after inserting update freespace
			newDirPage.updateFreeSpace(newDirEntry.directoryEntry_.entryNumber,
					newDataPage.getContFreeSpace());
			dirHeaderPage_->updateNumOfRecords(
					dirHeaderPage_->getNumberOfRecords() + 1);
			dirPage.setNextDirectoryPageNumber(newDirPage.getPageNumber());
			std::cout << "temp print, dirPAge Next Dir Page Num"
					<< dirPage.getNextDirectoryPageNumber()<<std::endl;
			return SUCCESS;
		}
		i = dirPage.getNextDirectoryPageNumber();
	}
	return SUCCESS;
}

void HeapFile::deleteRecord(const RIDStruct& rid) {
	DataPage dataPage(rid.pageNumber);
	dataPage.deleteRecord(rid);
}

STATUS_CODE HeapFile::updateRecord(const RIDStruct rid, char*& record,
		unsigned& recordLen) {
	DEBUG("in heapfile updateRecord"<<rid.pageNumber);
	DEBUG("recordLen="<<recordLen);
	DataPage dataPage(rid.pageNumber);
	unsigned curRecordLen = dataPage.getRecordLength(rid.slotNumber);
	if (recordLen <= curRecordLen) {
		dataPage.updateRecord(rid, record, recordLen);
		return SUCCESS;
	} else {
		RIDStruct forwardAddressRID;
		insertRecord(record, recordLen, forwardAddressRID);
		char *recWithFwdAddress = new char[9];
		recWithFwdAddress[0] = 'F';
		recWithFwdAddress[0] = 'W';
		recWithFwdAddress[0] = 'D';
		//3 bytes for "FWD"
		//6 bytes for rid
		DEBUG("curRID"<<rid.pageNumber<<"slot="<<rid.slotNumber)
		DEBUG("fwdRID"<<forwardAddressRID.pageNumber<<"slot="<<forwardAddressRID.slotNumber)
		memcpy(&recWithFwdAddress[3], &forwardAddressRID, RID_LEN);
		dataPage.updateRecord(rid, recWithFwdAddress, 9);
		delete recWithFwdAddress;
	}
	return SUCCESS;
}

void HeapFile::deleteFile() {
	DEBUG("in heapfile delete");
	BufferManager *bufMgr = BufferManager::getInstance();
	int firstDirPageNumber = dirHeaderPage_->getFirstDirectoryPageNumber();
	for (int i = firstDirPageNumber; i != -1;) {
		DirectoryPage dirPage(i);
		int numOfDEs = dirPage.getNumberOfDirectoryEntries();
		for (int j = 0; j < numOfDEs; j++) {
			DirectoryEntry dirEntry;
			dirPage.getDirectoryEntry(j, dirEntry);
			int tobeFreedPageNumber = dirEntry.getPageNumber();
			bufMgr->freePage(tobeFreedPageNumber);
		}
		i = dirPage.getNextDirectoryPageNumber();
		bufMgr->freePage(dirPage.getPageNumber());
	}

	//freeing header page
	bufMgr->freePage(dirHeaderPage_->getPageNumber());
	//find all indices call delete on that, it should free all those pages

}

int HeapFile::getNextRecordNumber() {
	return dirHeaderPage_->getNumberOfRecords();
}

/**
 * also handle fwd addresses here;
 * also the Datapage may be destructed here,handle that also
 */
void HeapFile::getRecord(const RIDStruct& rid, char*& record,
		unsigned& recordLen) {
	DataPage dataPage(rid.pageNumber);
	dataPage.getRecord(rid, record, recordLen);
}

int HeapFile::getNumOfRecords() {
	return dirHeaderPage_->getNumberOfRecords();
}

int HeapFile::getDirectoryHeaderPageNumber(){
	return dirHeaderPage_->getPageNumber();
}
