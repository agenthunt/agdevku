/*
 * DirectoryPage.h
 *
 *  Created on: Oct 2, 2009
 *      Author: shailesh
 */

#ifndef DIRECTORYPAGE_H_
#define DIRECTORYPAGE_H_

class DirectoryPage {
public:
	DirectoryPage();
	virtual ~DirectoryPage();
private:
	typedef struct DirectoryPageStructure{
		int pageNumber;
	} DirectoryPageStruct;
	DirectoryPageStruct directoryPage_;
};

#endif /* DIRECTORYPAGE_H_ */
