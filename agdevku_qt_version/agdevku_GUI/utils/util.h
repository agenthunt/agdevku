/*
 * util.h
 *
 *  Created on: Oct 8, 2009
 *      Author: shailesh
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
using std::string;
#include <sstream>
#include <vector>
#include "debug.h"
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "../global/ExternDefOfGlobalVariables.h"

using namespace std;
class Util {
public:


	static vector<string> split(string str, string splitter) {

		vector<string> res;

		for (unsigned int i = 0; i < str.length();) {
			int pos = str.find(splitter, i);
			string split_str = str.substr(i, pos - i);
			if (split_str != "") {
				res.push_back(split_str);
			}
			//cout << "i,pos=" << i << "," << pos << endl;
			//cout << str.substr(i, pos - i) << endl;
			i = pos + splitter.length();
			if (pos < 0) {
				break;
			}
		}

		return res;
	}

	static int string_to_int(std::string var) {
		DEBUG("string to int"<<var);
		int res = 0;
		istringstream mystream(var);
		mystream >> res;
		DEBUG("string to int,res="<<res);
		return res;
	}

	static string int_to_string(int var) {
		ostringstream mystream;
		mystream << var;
		return mystream.str();
	}

	static string char_to_string(char *var) {
		string res(var);
		return res;
	}

//	static void updateExecutionTime(double diff) {
//		//printf("It took you %.2lf seconds to type your name.\n", diff);
//
//		ostringstream mystream;
//		mystream << "Query executed in ";
//		mystream << diff << " secs" << endl;
//		const char *var = mystream.str().c_str();
//		strcpy(executionTime, var);
//	}
//
//	static void updateNumberOfRows(int numOfRows) {
//		ostringstream mystream;
//		mystream << "Number of Rows= ";
//		mystream << numOfRows << endl;
//		const char *var = mystream.str().c_str();
//		strcpy(numberOfRowsReturned, var);
//	}
//
//	static void startTimer() {
//		gettimeofday(&start, NULL);
//	}
//
//	static void endTimer() {
//		gettimeofday(&end, NULL);
//		seconds = end.tv_sec - start.tv_sec;
//		useconds = end.tv_usec - start.tv_usec;
//		mtime = ((seconds) * 1000 + useconds / 1000.0) + 0.5;
//		//printf("Elapsed time: %ld milliseconds\n", mtime);
//	}
//
//	static string getElapsedTimeInMilliSeconds(){
//		ostringstream mystream;
//		mystream << mtime;
//		return mystream.str();
//	}
//
//	static void updateExecutionTimeMessage(string msg){
//		strcpy(executionTime,msg.c_str());
//	}
//
//	static string convertLongToString(long val){
//		ostringstream mystream;
//		mystream << val;
//		return mystream.str();
//	}
};
#endif /* UTIL_H_ */
