/*
 * IntelliFormatter.h
 *
 *  Created on: 14-Dec-2009
 *      Author: shailesh
 */

#ifndef INTELLIFORMATTER_H_
#define INTELLIFORMATTER_H_
#include "../global/GlobalDefines.h"
#include "../diskmgr/DBMainHeaderPage.h"
#include "../heap/DirectoryHeaderPage.h"
#include "../heap/DirectoryPage.h"
#include "../heap/DataPage.h"
#include "../global/GeneralPageHeaderAccessor.h"
#include "../bplustree/IndexHeaderPage.h"
#include "../bplustree/IndexNode.h"
#include "../bplustree/LeafNode.h"
class IntelliFormatter {

public:
	static std::string format(char *data) {
		GeneralPageHeaderAccessor genPageAccessor(data);
		DBMainHeaderPage *dbMainHeaderPage;
		DirectoryHeaderPage *dirHeaderPage;
		DirectoryPage *dirPage;
		DataPage *dataPage;
		IndexHeaderPage * indexHeaderPage;
		IndexNode *indexNode;
		LeafNode *leafNode;
		std::string result;
		int pageType = genPageAccessor.getPageType();
		int pageNumber = genPageAccessor.getPageNumber();
		switch (pageType) {
		case TYPE_DB_MAIN_HEADER_PAGE:
			dbMainHeaderPage = new DBMainHeaderPage(pageNumber);
			result = dbMainHeaderPage->toString();
			delete dbMainHeaderPage;
			break;
		case TYPE_DIRECTORY_HEADER_PAGE:
			dirHeaderPage = new DirectoryHeaderPage(pageNumber);
			result = dirHeaderPage->toString();
			delete dirHeaderPage;
			break;
		case TYPE_DIRECTORY_PAGE:
			dirPage = new DirectoryPage(pageNumber);
			result = dirPage->toString();
			delete dirPage;
			break;
		case TYPE_DATA_PAGE:
			dataPage = new DataPage(pageNumber);
			result = dataPage->toString();
			delete dataPage;
			break;
		case TYPE_INDEX_HEADER_PAGE:
			indexHeaderPage = new IndexHeaderPage(pageNumber);
			result = indexHeaderPage->toString();
			delete indexHeaderPage;
			break;
		case TYPE_INDEX_PAGE:
			indexNode = new IndexNode(pageNumber);
			result = indexNode->toString();
			delete indexNode;
			break;
		case TYPE_LEAF_PAGE:
			leafNode = new LeafNode(pageNumber);
			result = leafNode->toString();
			delete leafNode;
			break;
		default:
			break;
		}

		return result;
	}

	static std::string formatUsingData(char *data) {
		GeneralPageHeaderAccessor genPageAccessor(data);
		DBMainHeaderPage *dbMainHeaderPage;
		DirectoryHeaderPage *dirHeaderPage;
		DirectoryPage *dirPage;
		DataPage *dataPage;
		std::string result;
		int pageType = genPageAccessor.getPageType();
		int pageNumber = genPageAccessor.getPageNumber();
		switch (pageType) {
		case TYPE_DB_MAIN_HEADER_PAGE:
			dbMainHeaderPage = new DBMainHeaderPage(data);
			result = dbMainHeaderPage->toString();
			delete dbMainHeaderPage;
			break;
		case TYPE_DIRECTORY_HEADER_PAGE:
			dirHeaderPage = new DirectoryHeaderPage(data);
			result = dirHeaderPage->toString();
			delete dirHeaderPage;
			break;
		case TYPE_DIRECTORY_PAGE:
			dirPage = new DirectoryPage(data);
			result = dirPage->toString();
			delete dirPage;
			break;
		case TYPE_DATA_PAGE:
			dataPage = new DataPage(data);
			result = dataPage->toString();
			break;
		default:
			break;
		}

		return result;
	}
};

#endif /* INTELLIFORMATTER_H_ */
