
#include "../executor/ExecuteStructures.h"


extern ExecuteStructures *executeStructures;

extern char currentlyOpenedDatabase[256];
extern char executionTime[512];
extern char numberOfRowsReturned[512];
extern int constructorErrorStatus;
extern bool BPLUSTREE_DEBUG;
extern bool HEAP_VERBOSE;
extern int ROWS_PER_PAGE;
extern bool COMMON_DEBUG;
extern bool USING_INDEX;
extern bool  USE_CLIENT_SIDE_SORT;
extern bool USE_MMAP;
extern struct timeval start, end;
extern long mtime, seconds, useconds;
