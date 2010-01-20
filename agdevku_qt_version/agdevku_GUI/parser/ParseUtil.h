/*
 * ParseUtil.h
 *
 *  Created on: 30-Nov-2009
 *      Author: shailesh
 */

#ifndef PARSEUTIL_H_
#define PARSEUTIL_H_
#include "heading.h"
#include <QString>
class ParseUtil {
public:
	ParseUtil();
	virtual ~ParseUtil();
	//STATUS_CODE parseQuery(const char *buf);
	STATUS_CODE parseQuery(QString buf);
};

#endif /* PARSEUTIL_H_ */
