#include "CommonHeaders.h" 

class Utilities
{
	public:
	static std::vector<std::string> split(const std::string &s, char delim);
	static void squeeZeWhiteSpace(std::string& s);
	static void fillQueueFromFile(std::queue<std::string>& queues, std::string _fname);
	static off_t getFileSize(std::string fileName);
	static bool isValidInputForCacheSize(std::string cSize);
	static void checkInputValidity(std::string cacheSize, std::string writerFile, std::string inFile);
};
