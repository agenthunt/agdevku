/*
 * HexFormatter.h
 *
 *  Created on: 05-Dec-2009
 *      Author: shailesh
 */

#ifndef HEXFORMATTER_H_
#define HEXFORMATTER_H_
#include <string>
#include <math.h>
#include <sstream>
#include <string.h>
#include <iomanip>
#include <ctype.h>
#include "../utils/debug.h"
using namespace std;
class HexFormatter {
public:
	static std::string format(char *data, int sizeInBytes,
			char *formattedString) {
		//		std::string result;
		//
		std::ostringstream mystream;
		int numOfRows = ceil(sizeInBytes / 16);
		int count = 0;

		mystream << uppercase;
		for (int i = 0; i < numOfRows && count < sizeInBytes; i++) {
			mystream << setfill('0') << setw(6);
			mystream << hex << i << "\t ";
			std::ostringstream charsAtEnd;
			for (int j = i * 16, k = 0; k < 16 && count < sizeInBytes; k++, j++) {
				mystream << setfill('0') << setw(2);
				//mystream << QString::number(data[j], 16).toStdString() << " ";
				short val = (short) data[j];
				val = val & 0xFF;
				if ((val >= 'A' && val <= 'Z') || (val >= 'a' && val <= 'z')
						|| (val >= '0' && val <= '9')) {
					//cout << val;
					charsAtEnd << data[j];
				} else {
					//cout << ".";
					charsAtEnd << ".";
				}
				charsAtEnd << " ";
				mystream << hex << val << " ";
				count++;
			}
			//DEBUG(charsAtEnd.str())
			mystream << "  " << charsAtEnd.str();
			charsAtEnd.clear();
			mystream << "\n";

		}

		//		formattedString = new char[36000];
		//		memset(formattedString,'\0',36000);
		//		for (int i = 0; i < numOfRows && count < sizeInBytes; i++) {
		//			sprintf(formattedString,"row=%d\t",i);
		//			for (int j = i * 16, k = 0; k < 16 && count < sizeInBytes; k++, j++) {
		//				//sprintf(formattedString,"%2x",data[j]);
		//				//sprintf(formattedString,"%s ",QString::number(data[j], 16).toStdString().c_str());
		//				count++;
		//			}
		//			//mystream << "\n";
		//			sprintf(formattedString,"\n");
		//
		//		}

		//		for (int i = 0; i < sizeInBytes; i++) {
		//			result.append(QString::number(data[i], 16).toStdString());
		//			if (i > 0 && i % 16 == 0) {
		//				result.append("\n");
		//			}
		//		}
		//tmp = new QString(*formattedString);


		//QString res(formattedString);
		//return tmp->toStdString();
		return mystream.str();
	}

};

#endif /* HEXFORMATTER_H_ */
