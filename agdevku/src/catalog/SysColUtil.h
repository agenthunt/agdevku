/*
 * SysColUtil.h
 *
 *  Created on: 15-Nov-2009
 *      Author: shailesh
 */

#ifndef SYSCOLUTIL_H_
#define SYSCOLUTIL_H_

class SysColUtil {
public:
	SysColUtil();
	virtual ~SysColUtil();
private:
	typedef struct SysColInitialStructure {
		int columnID;
		char columnName[32];
		//int physicalPosition; //lemme check if we need
		int columnPosition;
		int columnType;
	}SysColInitStruct;
};

#endif /* SYSCOLUTIL_H_ */
