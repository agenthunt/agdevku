/*
 * timer_util.h
 *
 *  Created on: 09-Jan-2010
 *      Author: shailesh
 */

#ifndef TIMER_UTIL_H_
#define TIMER_UTIL_H_

#include <iostream>
using std::string;
#include <sstream>
#include <vector>
#include "debug.h"
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
//#include "../global/ExternDefOfGlobalVariables.h"

using namespace std;
class TimerUtil {
public:
	struct timeval start, end;
	long mtime, seconds, useconds;
	void updateExecutionTime(double diff) {
		//printf("It took you %.2lf seconds to type your name.\n", diff);
		ostringstream mystream;
		mystream << "Query executed in ";
		mystream << diff << " secs" << endl;
		const char *var = mystream.str().c_str();
		strcpy(executionTime, var);
	}

	void updateNumberOfRows(int numOfRows) {
		ostringstream mystream;
		mystream << "Number of Rows= ";
		mystream << numOfRows << endl;
		const char *var = mystream.str().c_str();
		strcpy(numberOfRowsReturned, var);
	}

	void startTimer() {
		gettimeofday(&start, NULL);
	}

	void endTimer() {
		gettimeofday(&end, NULL);
		seconds = end.tv_sec - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;
		mtime = ((seconds) * 1000 + useconds / 1000.0) + 0.5;
		//printf("Elapsed time: %ld milliseconds\n", mtime);
	}

	string getElapsedTimeInSeconds() {
		ostringstream mystream;
		mystream << seconds;
		return mystream.str();
	}

	string getElapsedTimeInMilliSeconds() {
		ostringstream mystream;
		mystream << mtime;
		return mystream.str();
	}

	void updateExecutionTimeMessage(string msg) {
		strcpy(executionTime, msg.c_str());
	}

};

#endif /* TIMER_UTIL_H_ */
