/*
 * debug.h
 *
 *  Created on: Oct 8, 2009
 *      Author: shailesh
 */

#ifndef DEBUG_H_
#define DEBUG_H_
#define DEBUG_FLAG  1
#include <iostream>

#ifndef DEBUG_FLAG
#define DEBUG(x)
#else
#define DEBUG(x) \
       std::cout <<"File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;
#endif

#endif /* DEBUG_H_ */
