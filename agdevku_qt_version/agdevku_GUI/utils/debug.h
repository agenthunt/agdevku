/*
 * debug.h
 *
 *  Created on: Oct 8, 2009
 *      Author: shailesh
 */

#ifndef DEBUG_H_
#define DEBUG_H_
//#define DEBUG_FLAG  1
#define HEX_DEBUG_FLAG 1
#include <iostream>
#include "../global/ExternDefOfGlobalVariables.h"
//#ifndef DEBUG_FLAG
//#define DEBUG(x)
//#else
#define DEBUG(x) \
if(COMMON_DEBUG == true){ \
       std::cout <<"File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;\
std::cout.flush();\
}
//#endif

#ifndef HEX_DEBUG_FLAG
#define DEBUG_HEX(x)
#else
#define DEBUG_HEX(x) \
       std::cout <<"File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<<std::hex<< x << std::endl;\
std::cout.flush();
#endif

#define DEBUG_B(x) \
if(BPLUSTREE_DEBUG == true){ \
	std::cout <<x << std::endl; \
}

#define HEAP_VERBOSE_MSG(x) \
if(HEAP_VERBOSE == true){ \
	std::cout <<x << std::endl; \
}

#endif /* DEBUG_H_ */
