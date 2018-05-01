#ifndef    _THREADS_HANDLER_H
#define    _THREADS_HANDLER_H		 

#include "Utilities.h" 

using namespace std;
class Readers;
class Writers;
class ThreadsHandler
{
	private:
	shared_ptr<Readers> _readers;
	shared_ptr<Writers> _writers;
	static std::shared_timed_mutex _cacheLock;
	static std::shared_timed_mutex _diskLock;
	static std::shared_timed_mutex _cacheOrDiskLock;
	static std::shared_timed_mutex _createFileLock;
	static std::vector<std::string> _infoFileContentsOnDisk;
	static MemCache _memCache;
	static CachedItemsList _cachedItemsTracker;
	static C_UINT _cacheSize;
	std::string _inputCacheSize;
	std::string _readerFile ;
	std::string _writerFile;
	std::string _inFile ;
	static C_LINT _inFileSize;
	public:
	ThreadsHandler(std::string cacheSize, std::string readerFile, std::string writerFile, std::string inFile);
	static void updateDiskContentsAndCache(std::string lineNum, std::string contents);
	static std::string readRecordFromCacheOrDisk(std::string lineNum);
	void startProcess();
	static void fillVectorFromFile(std::vector<std::string>& vec, std::string name, bool isInfile=false);
	static void createReaderFileFromVector(std::vector<std::string>& vec, std::string name);
	void createOrOverWriteinFile(std::vector<string>& v, std::string inFileName);
};

#endif
