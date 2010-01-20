/*
 * HeapFilePage.h
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#ifndef DATAPAGE_H_
#define DATAPAGE_H_
#include "../global/GlobalStructures.h"
#include <string>
#include <sstream>
class DataPage {
public:
	typedef struct SlotEntryStructure {
		int recordOffset;
		unsigned recordLength;
		std::string toString() {
			std::ostringstream mystream;
			mystream << "RECORD OFFSET: " << recordOffset << "\n";
			mystream << "RECORD LENGTH: " << recordLength << "\n";
			return mystream.str();
		}
	} SlotEntryStruct;
	DataPage();
	DataPage(char *pageData);
	DataPage(int pageNumber);
	virtual ~DataPage();
	//void createDataPage();
	void createDataPage(int dirHeaderPageNumber,int directoryPageNumber);
	int getPageNumber();
	int insertRecord(char *record, unsigned recordLen);
	void deleteRecord(const RIDStruct& rid);
	void updateRecord(const RIDStruct rid, char*& record, unsigned recordLen);
	unsigned getContFreeSpace();
	int getContFreeSpacePointer();
	unsigned getRecordLength(int slotNumber);
	void getRecord(const RIDStruct &rid, char*& record, unsigned& recordLen);
	int getNumOfSlots();
	void updateNumOfSlots(int numOfSlots);
	int getDirectoryHeaderPageNumber();
	void setDirectoryEntryNumber(int directoryEntryNumber);
	int getDirectoryPageNumber();
	int getDirectoryEntryNumber();
	std::string toString();
	bool isValidSlotNum(int slotNumber);
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
		int dirHeaderPageNumber;
		int directoryPageNumber;
		int directoryEntryNumber;
	} DataPageHeaderStruct; // 36 bytes of headerinfo

	DataPageHeaderStruct dataPageHeader_;
	char *pageData_;
	bool haveIPinnedThePage;
	bool isDirty;
};

#endif /* DATAPAGE_H_ */
