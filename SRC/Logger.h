#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>

class Logger {
public:
	Logger();
	Logger(std::string fn);

	std::string getFileName();
	void setFileName(std::string fn);
	void debug(std::string msg);

private:
	std::string directory = "../LOGS/";
	std::string fileName = "";
	std::string currentTime();
};

#endif