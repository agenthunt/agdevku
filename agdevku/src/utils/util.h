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

using namespace std;
class Util{
public:


	static	vector<string> split(string str, string splitter) {

		vector<string> res;

		for (unsigned int i = 0; i < str.length();) {
			int pos = str.find(splitter, i);

			res.push_back(str.substr(i, pos - i));
			//cout << "i,pos=" << i << "," << pos << endl;
			//cout << str.substr(i, pos - i) << endl;
			i = pos + splitter.length();
			if (pos < 0) {
				break;
			}
		}

		return res;
	}

	static int string_to_int(std::string var){
		int res = 0;
		istringstream mystream(var);
		mystream >> res;
		return res;
	}

	static string int_to_string(int var){
		ostringstream mystream;
		mystream<<var;
		return mystream.str();
	}
};
#endif /* UTIL_H_ */
