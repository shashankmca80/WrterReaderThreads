#ifndef    _READERS_H
#define    _READERS_H 
#include "HandleThreads.h"

class Readers
{
	private:
	std::string _mainReaderFile;
	std::queue<std::string> _readerQueue;
	std::vector<std::thread> _readerThreads;
	std::mutex getReaderLock;
	long int _totalReaderThreads;
	long int _pendingReaderThread;
	public:
	Readers(std::string mainReaderFile);
	void processQueuedReaders();
	void startProcessingReaders(long int n);
	long int getTotalReaderThreads();
	void queueUpReaders();
	void joinReaderThreads();
};

#endif
