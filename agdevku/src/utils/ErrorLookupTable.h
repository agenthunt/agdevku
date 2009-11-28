#include "../global/StatusCodes.h"

class ErrorLookupTable {
public:
	static std::string lookup(int errorNumber) {
		switch (errorNumber) {
		case DATABASE_ALREADY_EXISTS:
			return "DATABASE_ALREADY_EXISTS";
		case SUCCESS:
			return "SUCCESS";
		default:
			return "UNKNOWN ERROR NUMBER";
		}
	}
};
