/*
 * HeapFilePage.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "DataPage.h"
#include "../bufmgr/BufferManager.h"
#include <string.h>
DataPage::DataPage() {
	// TODO Auto-generated constructor stub

}

DataPage::DataPage(int pageNumber) {
	BufferManager *bufMgr = BufferManager::getInstance();
	bufMgr->pinAndGetPage(pageNumber, pageData_);
	memcpy(&dataPageHeader_, pageData_, sizeof(DataPageHeaderStruct));
}

DataPage::~DataPage() {
	// TODO Auto-generated destructor stub
	BufferManager *bufMgr = BufferManager::getInstance();
	bufMgr->unPinPage(dataPageHeader_.genPageHeader.pageNumber, true);
}

void DataPage::createDataPage() {
	BufferManager *bufMgr = BufferManager::getInstance();
	int pageNumber;
	bufMgr->newPage(pageNumber, pageData_);
	memcpy(&dataPageHeader_, pageData_, sizeof(DataPageHeaderStruct));
	dataPageHeader_.numOfSlots = 0;
	dataPageHeader_.maxContFreeSpace = bufMgr->getCurrentlyUsingPageSize()
			- sizeof(DataPageHeaderStruct) - dataPageHeader_.numOfSlots
			* sizeof(SlotEntryStruct);
	dataPageHeader_.contFreeSpacePointer = sizeof(DataPageHeaderStruct);
	memcpy(pageData_, &dataPageHeader_, sizeof(DataPageHeaderStruct));
}

int DataPage::getPageNumber() {
	return dataPageHeader_.genPageHeader.pageNumber;
}

void DataPage::getSlotEntry(int slotNumber, SlotEntryStruct& slotEntry) {
	BufferManager *bufMgr = BufferManager::getInstance();
	int offset = bufMgr->getCurrentlyUsingPageSize() - (slotNumber + 1)
			* sizeof(SlotEntryStruct);
	//slotNumber+1 because I'm going in reverse way.
	memcpy(&slotEntry, &pageData_[offset], sizeof(SlotEntryStruct));
}

void DataPage::updateSlotEntry(int slotNumber, SlotEntryStruct& slotEntry) {
	BufferManager *bufMgr = BufferManager::getInstance();
	int offset = bufMgr->getCurrentlyUsingPageSize() - (slotNumber + 1)
			* sizeof(SlotEntryStruct);
	//slotNumber+1 because I'm going in reverse way.
	memcpy(&pageData_[offset], &slotEntry, sizeof(SlotEntryStruct));
}

void DataPage::addSlotEntry(SlotEntryStruct& slotEntry) {
	BufferManager *bufMgr = BufferManager::getInstance();
	int offset = bufMgr->getCurrentlyUsingPageSize()
			- (dataPageHeader_.numOfSlots + 1) * sizeof(SlotEntryStruct);
	memcpy(&pageData_[offset], &slotEntry, sizeof(SlotEntryStruct));
	dataPageHeader_.numOfSlots = dataPageHeader_.numOfSlots + 1;
	dataPageHeader_.contFreeSpacePointer = dataPageHeader_.contFreeSpacePointer
			+ slotEntry.recordLength;
	memcpy(pageData_, &dataPageHeader_, sizeof(DataPageHeaderStruct));
}

int DataPage::insertRecord(char *record, unsigned recordLen) {
	int i = 0;
	for (i = 0; i < dataPageHeader_.numOfSlots; i++) {
		SlotEntryStruct slotEntry;
		getSlotEntry(i, slotEntry);
		if (slotEntry.recordOffset < 0 && recordLen <= slotEntry.recordLength) {
			int offset = -slotEntry.recordOffset;
			memcpy(&pageData_[offset], record, recordLen * sizeof(char));
			slotEntry.recordOffset = slotEntry.recordOffset + recordLen;
			slotEntry.recordLength = slotEntry.recordLength - recordLen;
			updateSlotEntry(i, slotEntry);
			calcAndUpdateMaxContFreeSpace();
			return i;
		}
	}
	//there are no free slots, add in CFS , and add a slot
	memcpy(&pageData_[dataPageHeader_.contFreeSpacePointer], record, recordLen
			* sizeof(char));
	SlotEntryStruct slotEntry = { dataPageHeader_.contFreeSpacePointer,
			recordLen };
	addSlotEntry(slotEntry);
	calcAndUpdateMaxContFreeSpace();
	return i;
}

void DataPage::calcAndUpdateMaxContFreeSpace() {

	for (int i = 0; i < dataPageHeader_.numOfSlots; i++) {
		SlotEntryStruct slotEntry;
		getSlotEntry(i, slotEntry);
		if (slotEntry.recordOffset < 0) {
			if (slotEntry.recordLength > dataPageHeader_.maxContFreeSpace) {
				dataPageHeader_.maxContFreeSpace = slotEntry.recordLength;
				dataPageHeader_.contFreeSpacePointer = -slotEntry.recordOffset;
			}
		}
	}
	memcpy(pageData_, &dataPageHeader_, sizeof(DataPageHeaderStruct));
}

unsigned DataPage::getContFreeSpace() {
	memcpy(&dataPageHeader_, pageData_, sizeof(DataPageHeaderStruct));
	return dataPageHeader_.maxContFreeSpace;
}

void DataPage::deleteRecord(const RIDStruct& rid) {
	SlotEntryStruct slotEntry;
	slotEntry.recordOffset = -slotEntry.recordOffset;
	updateSlotEntry(rid.slotNumber, slotEntry);
	calcAndUpdateMaxContFreeSpace();
}

void DataPage::updateRecord(const RIDStruct rid, char *record,
		unsigned recordLen) {
	SlotEntryStruct slotEntry;
	getSlotEntry(rid.slotNumber, slotEntry);
	int offset = slotEntry.recordOffset;
	memcpy(&pageData_[offset], record, recordLen * sizeof(char));
	slotEntry.recordOffset = slotEntry.recordOffset + recordLen;
	slotEntry.recordLength = slotEntry.recordLength - recordLen;
	updateSlotEntry(rid.slotNumber, slotEntry);
	calcAndUpdateMaxContFreeSpace();
}

unsigned DataPage::getRecordLength(int slotNumber) {
	SlotEntryStruct slotEntry;
	getSlotEntry(slotNumber, slotEntry);
	return slotEntry.recordLength;
}

void DataPage::getRecord(const RIDStruct &rid,char *record, unsigned& recordLen) {
	SlotEntryStruct slotEntry;
	getSlotEntry(rid.slotNumber, slotEntry);
	recordLen = slotEntry.recordLength;
	record = &pageData_[slotEntry.recordOffset];
}
