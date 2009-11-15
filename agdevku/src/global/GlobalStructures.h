/*
 * GlobalStructures.h
 *
 *  Created on: Sep 30, 2009
 *      Author: shailesh
 */

#ifndef GLOBALSTRUCTURES_H_
#define GLOBALSTRUCTURES_H_

typedef struct GeneralPageHeaderStructure {
	int pageNumber;
	int nextPageNumber;
} GeneralPageHeaderStruct;

typedef struct RIDStructure {
	int pageNumber;
	short slotNumber;
} RIDStruct;

typedef int STATUS_CODE;

#endif /* GLOBALSTRUCTURES_H_ */
