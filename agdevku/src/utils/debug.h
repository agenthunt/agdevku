/*
 * debug.h
 *
 *  Created on: Oct 8, 2009
 *      Author: shailesh
 */

#ifndef DEBUG_H_
#define DEBUG_H_
#define DEBUG_FLAG  1
#define HEX_DEBUG_FLAG 1
#include <iostream>

#ifndef DEBUG_FLAG
#define DEBUG(x)
#else
#define DEBUG(x) \
       std::cout <<"File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<< x << std::endl;\
std::cout.flush();
#endif

#ifndef HEX_DEBUG_FLAG
#define DEBUG_HEX(x)
#else
#define DEBUG_HEX(x) \
       std::cout <<"File:"<< __FILE__ <<",Line:" <<__LINE__ <<",msg:"<<std::hex<< x << std::endl;\
std::cout.flush();
#endif

#endif /* DEBUG_H_ */
