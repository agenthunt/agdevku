/*
 * HeapFilePage.h
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#ifndef DATAPAGE_H_
#define DATAPAGE_H_
#include "../global/GlobalStructures.h"
class DataPage {
public:
	typedef struct SlotEntryStructure {
		int recordOffset;
		unsigned recordLength;
	} SlotEntryStruct;
	DataPage();
	DataPage(int pageNumber);
	virtual ~DataPage();
	void createDataPage();
	int getPageNumber();
	int insertRecord(char *record, unsigned recordLen);
	void deleteRecord(const RIDStruct& rid);
	void updateRecord(const RIDStruct rid,char *record,unsigned recordLen);
	unsigned getContFreeSpace();
	unsigned getRecordLength(int slotNumber);
	void getRecord(const RIDStruct &rid,char *record,unsigned& recordLen);
private:
	void updateSlotEntry(int slotNumber, SlotEntryStruct& slotEntry);
	void getSlotEntry(int slotNumber, SlotEntryStruct& slotEntry);
	void addSlotEntry(SlotEntryStruct& slotEntry);
	void calcAndUpdateMaxContFreeSpace();
	typedef struct DataPageHeaderStructure {
		GeneralPageHeaderStructure genPageHeader;
		int numOfSlots;
		unsigned maxContFreeSpace;
		int contFreeSpacePointer;
	} DataPageHeaderStruct;

	DataPageHeaderStruct dataPageHeader_;
	char *pageData_;
};

#endif /* DATAPAGE_H_ */
