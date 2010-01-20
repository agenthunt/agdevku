/*
 * GlobalStructures.h
 *
 *  Created on: Sep 30, 2009
 *      Author: shailesh
 */

#ifndef GLOBALSTRUCTURES_H_
#define GLOBALSTRUCTURES_H_
#include <string>
#include <sstream>
typedef struct GeneralPageHeaderStructure {
	int pageNumber;
	int nextPageNumber;
	int pageType;
} GeneralPageHeaderStruct;

typedef struct RIDStructure {
	int pageNumber;
	short slotNumber;
	struct RIDStructure& operator=(const struct RIDStructure& rid) {
		pageNumber = rid.pageNumber;
		slotNumber = rid.slotNumber;
		return *this;
	}

	bool operator<(const struct RIDStructure& rid) const {
		if (pageNumber < rid.pageNumber) {
			return true;
		} else if (pageNumber == rid.pageNumber) {
			return slotNumber < rid.slotNumber;
		}
		return false;
	}

	bool operator>(const struct RIDStructure& rid) const {
		if (pageNumber > rid.pageNumber) {
			return true;
		} else if (pageNumber == rid.pageNumber) {
			return slotNumber > rid.slotNumber;
		}
		return false;
	}

	bool operator==(const struct RIDStructure& rid) const {
		if (pageNumber == rid.pageNumber && pageNumber == rid.pageNumber) {
			return true;
		}
		return false;
	}

	std::string toString() {
		std::ostringstream mystream;
		mystream << "RID={pageNumber=" << this->pageNumber << ",slotNumber="
				<< this->slotNumber << "}";
		return mystream.str();
	}

} RIDStruct;

typedef int STATUS_CODE;

#endif /* GLOBALSTRUCTURES_H_ */
