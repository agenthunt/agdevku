/*
 * HeapFilePage.cpp
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#include "DataPage.h"
#include "../bufmgr/BufferManager.h"
#include "../global/GlobalDefines.h"
#include "../global/GeneralPageHeaderAccessor.h"
#include "../catalog/CatalogUtil.h"
#include "../utils/debug.h"
#include "Tuple.h"
#include <string.h>
#include <sstream>
#include <iomanip>
#include <assert.h>

DataPage::DataPage() {
	haveIPinnedThePage = false;
	isDirty = false;
}

DataPage::DataPage(char *pageData) {
	haveIPinnedThePage = false;
	isDirty = false;
	pageData_ = pageData;
	memcpy(&dataPageHeader_, pageData_, sizeof(DataPageHeaderStruct));
}

DataPage::DataPage(int pageNumber) {
	haveIPinnedThePage = true;
	isDirty = false;
	BufferManager *bufMgr = BufferManager::getInstance();
	bufMgr->pinAndGetPage(pageNumber, pageData_);
	memcpy(&dataPageHeader_, pageData_, sizeof(DataPageHeaderStruct));
}

DataPage::~DataPage() {
	// TODO Auto-generated destructor stub
	if (haveIPinnedThePage == true) {
		BufferManager *bufMgr = BufferManager::getInstance();
		bufMgr->unPinPage(dataPageHeader_.genPageHeader.pageNumber, isDirty);
	}
}

void DataPage::createDataPage(int dirHeaderPageNumber, int directoryPageNumber) {
	BufferManager *bufMgr = BufferManager::getInstance();
	int pageNumber;
	bufMgr->newPage(pageNumber, pageData_);
	haveIPinnedThePage = true;
	isDirty = true;
	memcpy(&dataPageHeader_, pageData_, sizeof(DataPageHeaderStruct));
	dataPageHeader_.genPageHeader.pageType = TYPE_DATA_PAGE;
	dataPageHeader_.numOfSlots = 0;
	dataPageHeader_.maxContFreeSpace = bufMgr->getCurrentlyUsingPageSize()
			- sizeof(DataPageHeaderStruct) - (1
			* sizeof(SlotEntryStruct));//reserve it earlier
	dataPageHeader_.contFreeSpacePointer = sizeof(DataPageHeaderStruct);
	dataPageHeader_.dirHeaderPageNumber = dirHeaderPageNumber;
	dataPageHeader_.directoryPageNumber = directoryPageNumber;
	dataPageHeader_.directoryEntryNumber = -1;
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
	isDirty = true;
}

void DataPage::addSlotEntry(SlotEntryStruct& slotEntry) {
	BufferManager *bufMgr = BufferManager::getInstance();
	int offset = bufMgr->getCurrentlyUsingPageSize()
			- (dataPageHeader_.numOfSlots + 1) * sizeof(SlotEntryStruct);
	memcpy(&pageData_[offset], &slotEntry, sizeof(SlotEntryStruct));
	dataPageHeader_.numOfSlots = dataPageHeader_.numOfSlots + 1;
	dataPageHeader_.contFreeSpacePointer = dataPageHeader_.contFreeSpacePointer
			+ slotEntry.recordLength;
	DEBUG("contFreeSpacePointer="<<dataPageHeader_.contFreeSpacePointer);
	assert(dataPageHeader_.contFreeSpacePointer < 8192);

	dataPageHeader_.maxContFreeSpace = dataPageHeader_.maxContFreeSpace
			- slotEntry.recordLength- sizeof(SlotEntryStruct);
	DEBUG("maxContFreeSpace="<<dataPageHeader_.maxContFreeSpace);
	assert(dataPageHeader_.maxContFreeSpace < 8192);

	memcpy(pageData_, &dataPageHeader_, sizeof(DataPageHeaderStruct));
	isDirty = true;
}

int DataPage::insertRecord(char *record, unsigned recordLen) {
	int i = 0;
	for (i = 0; i < dataPageHeader_.numOfSlots; i++) {
		SlotEntryStruct slotEntry;
		getSlotEntry(i, slotEntry);
		if (slotEntry.recordOffset < 0 && recordLen <= slotEntry.recordLength) {
			int offset = -slotEntry.recordOffset;
			memcpy(&pageData_[offset], record, recordLen * sizeof(char));
			slotEntry.recordOffset = -slotEntry.recordOffset;
			if (recordLen < slotEntry.recordLength) {
				SlotEntryStruct leftOverSlotEntry;
				leftOverSlotEntry.recordOffset = slotEntry.recordOffset
						+ recordLen;
				leftOverSlotEntry.recordLength = slotEntry.recordLength
						- recordLen;
				addSlotEntry(leftOverSlotEntry);
			}
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
	isDirty = true;
	return i;
}

void DataPage::calcAndUpdateMaxContFreeSpace() {
	// go through free slots and check for maximum contFreespace
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
	//have you excluded the space occupied by the slot entries?
	memcpy(pageData_, &dataPageHeader_, sizeof(DataPageHeaderStruct));
	isDirty = true;
}

unsigned DataPage::getContFreeSpace() {
	memcpy(&dataPageHeader_, pageData_, sizeof(DataPageHeaderStruct));
	return dataPageHeader_.maxContFreeSpace;
}

void DataPage::updateNumOfSlots(int numOfSlots) {
	dataPageHeader_.numOfSlots = numOfSlots;
	memcpy(pageData_, &dataPageHeader_, sizeof(DataPageHeaderStruct));
	isDirty = true;
}

void DataPage::deleteRecord(const RIDStruct& rid) {
	HEAP_VERBOSE_MSG("delete Record with RID="<<((RIDStruct)rid).toString());
	SlotEntryStruct slotEntry;
	getSlotEntry(rid.slotNumber, slotEntry);
	slotEntry.recordOffset = -slotEntry.recordOffset;
	updateSlotEntry(rid.slotNumber, slotEntry);
	//should you update the slots. no right?
	//updateNumOfSlots(getNumOfSlots() - 1);
	calcAndUpdateMaxContFreeSpace();
	isDirty = true;
}

void DataPage::updateRecord(const RIDStruct rid, char*& record,
		unsigned recordLen) {
	DEBUG("DataPage,rid="<<((RIDStruct)rid).toString());
	DEBUG("recordLen="<<recordLen);
	SlotEntryStruct slotEntry;
	getSlotEntry(rid.slotNumber, slotEntry);
	DEBUG("before,slot entry"<<slotEntry.toString());
	int offset = slotEntry.recordOffset;
	memcpy(&pageData_[offset], record, recordLen * sizeof(char));

	//crap why I did add these two lines
	//slotEntry.recordOffset = slotEntry.recordOffset + recordLen;
	//slotEntry.recordLength = slotEntry.recordLength - recordLen;

	DEBUG("after,slot entry"<<slotEntry.toString());
	updateSlotEntry(rid.slotNumber, slotEntry);
	calcAndUpdateMaxContFreeSpace();
	isDirty = true;
	DEBUG("DataPage, after updateRecord"<<toString());
}

unsigned DataPage::getRecordLength(int slotNumber) {
	SlotEntryStruct slotEntry;
	getSlotEntry(slotNumber, slotEntry);
	return slotEntry.recordLength;
}

bool DataPage::isValidSlotNum(int slotNumber) {
	SlotEntryStruct slotEntry;
	getSlotEntry(slotNumber, slotEntry);
	if (slotEntry.recordOffset < 0) {
		return false;
	}
	return true;
}

void DataPage::getRecord(const RIDStruct &rid, char*& record,
		unsigned& recordLen) {
	SlotEntryStruct slotEntry;
	getSlotEntry(rid.slotNumber, slotEntry);
	recordLen = slotEntry.recordLength;
	record = &pageData_[slotEntry.recordOffset];
}

int DataPage::getNumOfSlots() {
	memcpy(&dataPageHeader_, pageData_, sizeof(DataPageHeaderStruct));
	return dataPageHeader_.numOfSlots;
}

int DataPage::getContFreeSpacePointer() {
	memcpy(&dataPageHeader_, pageData_, sizeof(DataPageHeaderStruct));
	return dataPageHeader_.contFreeSpacePointer;
}

int DataPage::getDirectoryHeaderPageNumber() {
	memcpy(&dataPageHeader_, pageData_, sizeof(DataPageHeaderStruct));
	return dataPageHeader_.dirHeaderPageNumber;
}

void DataPage::setDirectoryEntryNumber(int directoryEntryNumber) {
	dataPageHeader_.directoryEntryNumber = directoryEntryNumber;
	memcpy(pageData_, &dataPageHeader_, sizeof(DataPageHeaderStruct));
	isDirty = true;
}

int DataPage::getDirectoryPageNumber() {
	memcpy(&dataPageHeader_, pageData_, sizeof(DataPageHeaderStruct));
	return dataPageHeader_.directoryPageNumber;
}

int DataPage::getDirectoryEntryNumber() {
	memcpy(&dataPageHeader_, pageData_, sizeof(DataPageHeaderStruct));
	return dataPageHeader_.directoryEntryNumber;
}

std::string DataPage::toString() {
	DEBUG("inside DataPage, toString");
	std::ostringstream mystream;
	GeneralPageHeaderAccessor genPageAccessor(pageData_);
	mystream << genPageAccessor.toString();
	mystream << "DIRECTORY HEADER PAGE NO: " << getDirectoryHeaderPageNumber()
			<< "\n";
	mystream << "DIRECTORY PAGE NO: " << getDirectoryPageNumber() << "\n";
	mystream << "DIRECTORY ENTRY NO: " << getDirectoryEntryNumber() << "\n";
	mystream << "NUMBER OF SLOTS: " << getNumOfSlots() << "\n";
	mystream << "MAX CONTIGUOUS FREE SPACE: " << getContFreeSpace() << "\n";
	mystream << "CONTIGUOUS FREE SPACE PTR: " << getContFreeSpacePointer()
			<< "\n";

	int numOfSlots = getNumOfSlots();
	int pageNumber = getPageNumber();

	mystream << "**************SLOT ENTRIES*******************\n";
	for (int i = 0; i < numOfSlots; i++) {
		SlotEntryStruct slotEntry;
		getSlotEntry(i, slotEntry);
		mystream << slotEntry.toString() << "\n";
	}
	mystream << "**************SLOT ENTRIES*******************\n";

	mystream << "**********RECORDS USING META INFO*******************\n";
	CatalogUtil catalogUtil;
	Schema thisPageSchema;
	int error = catalogUtil.getSchema(getDirectoryHeaderPageNumber(),
			thisPageSchema);
	if (error == TABLE_NOT_FOUND) {
		DEBUG("TABLE NOT FOUND");
	}

	mystream << std::uppercase;
	DEBUG("inside DataPage, toString, numOfSlots=" << numOfSlots);
	for (int i = 0; i < thisPageSchema.numOfColumns; ++i) {
		mystream << std::setfill(' ') << std::setw(20);
		mystream << thisPageSchema.columnNames.at(i) << " ";
	}
	mystream << "\n";
	for (int i = 0; i < numOfSlots; i++) {
		const RIDStruct rid = { pageNumber, i };
		char *record;
		unsigned recordLen;
		getRecord(rid, record, recordLen);
		Tuple tuple(thisPageSchema, record, recordLen);
		mystream << tuple.toString();
		mystream << "\n";
	}
	return mystream.str();
}

