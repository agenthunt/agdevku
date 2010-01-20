/*
 * BPlusTreeUtil.h
 *
 *  Created on: 14-Nov-2009
 *      Author: ravindra
 */

#ifndef BPLUSTREEUTIL_H_
#define BPLUSTREEUTIL_H_
#include "IndexHeaderPage.h"
#include<string>
class BPlusTreeUtil {
public:
	BPlusTreeUtil();
	virtual ~BPlusTreeUtil();
	static int keyCompare(const char* key1, const char* key2,
			IndexHeaderPage *indexHeader_);
	static void displayKey(const char* key, IndexHeaderPage *indexHeaderPage_);
	static void displayType(int type);
	//	static int validation(const std::string& input);
	static int validation(int input);
	static std::string KeytoString(const char* key,IndexHeaderPage *indexHeaderPage_);
};

#endif /* BPLUSTREEUTIL_H_ */
