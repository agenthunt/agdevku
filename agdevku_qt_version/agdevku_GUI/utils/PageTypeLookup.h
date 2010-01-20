/*
 * PageTypeLookup.h
 *
 *  Created on: 14-Dec-2009
 *      Author: shailesh
 */

#ifndef PAGETYPELOOKUP_H_
#define PAGETYPELOOKUP_H_
#include <string>
#include "../global/GlobalDefines.h"
class PageTypeLookup {
public:
	static std::string lookup(int pageType) {
		switch (pageType) {
		case TYPE_DB_MAIN_HEADER_PAGE:
			return "DB_MAIN_HEADER_PAGE";
		case TYPE_DIRECTORY_HEADER_PAGE:
			return "DIRECTORY_HEADER_PAGE";
		case TYPE_DIRECTORY_PAGE:
			return "DIRECTORY_PAGE";
		case TYPE_DATA_PAGE:
			return "DATA_PAGE";
		case TYPE_INDEX_HEADER_PAGE:
			return "INDEX_HEADER_PAGE";
		case TYPE_INDEX_PAGE:
			return "INDEX_PAGE";
		case TYPE_LEAF_PAGE:
			return "LEAF_PAGE";
		case TYPE_UNKNOWN_PAGE:
			return "UNKNOWN PAGE TYPE";
		default:
			return "PAGE TYPE FIELD CORRUPTED";
		}
	}
};

#endif /* PAGETYPELOOKUP_H_ */
