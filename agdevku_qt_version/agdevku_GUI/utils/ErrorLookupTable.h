#include "../global/StatusCodes.h"

class ErrorLookupTable {
public:
	static std::string lookup(int errorNumber) {
		switch (errorNumber) {
		case DATABASE_ALREADY_EXISTS:
			return "DATABASE_ALREADY_EXISTS";
		case PARSE_FAILED:
			return "SYNTAX ERROR";
		case DATABASE_NOT_OPEN:
			return "DATABASE_NOT_OPEN";
		case INVALID_PAGE:
			return "INVALID PAGE";
		case TABLE_EXISTS:
			return "TABLE EXISTS";
		case TABLE_NOT_FOUND:
			return "TABLE NOT FOUND";
		case DISK_PAGE_READ_ERROR:
			return "DISK_PAGE_READ_ERROR";
		case DATABASE_DOES_NOT_EXIST:
			return "DATABASE_DOES_NOT_EXIST";
		case CLOSE_OPENED_DATABASE:
			return "CLOSE_OPENED_DATABASE";
		case DISK_FILE_OPEN_ERROR:
			return "DISK_FILE_OPEN_ERROR";
		case DISK_PAGE_WRITE_ERROR:
			return "DISK_PAGE_WRITE_ERROR";
		case SUCCESS:
			return "SUCCESS";

		default:
			return "UNKNOWN ERROR NUMBER";
		}
	}
};
