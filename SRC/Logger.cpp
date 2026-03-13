#include "Logger.h"
#include <fstream>
#include <iostream>
#include <ctime>

Logger::Logger() {}
Logger::Logger(std::string fn) {
	fileName = fn;
}

std::string Logger::getFileName() {
	return fileName;
}

void Logger::setFileName(std::string fn) {
	fileName = fn;
}

void Logger::debug(std::string msg) {
	// Check if log filename is empty
	// Return if it is
	if (fileName.empty()) { return;  }
	// Open log file in append mode
	std::ofstream LogFile(directory + fileName, std::ios::app);
	// Write debug message to log file and make new line for next message
	LogFile << "[["
		<< currentTime()
		<< "]]: "
		<< msg
		<< std::endl;
	// Close log file
	LogFile.close();
}

std::string Logger::currentTime() {
	// Make buffer
	char buffer[100];
	// Generate current timestamp
	time_t timestamp;
	time(&timestamp);
	// Writes timestamp into buffer
	ctime_s(buffer, sizeof(buffer), &timestamp);
	// Converts buffer to string
	std::string sBuffer = std::string(buffer);
	// Checks for and removes newlines
	if (!sBuffer.empty() && sBuffer[sBuffer.length() - 1] == '\n') {
		sBuffer.erase(sBuffer.length() - 1);
	}
	// Returns buffer as string
	return sBuffer;
}