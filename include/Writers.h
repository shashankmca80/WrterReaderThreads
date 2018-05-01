#ifndef    _WRITERS_H
#define    _WRITERS_H
#include "HandleThreads.h"

class Writers
{
	private:
	std::string _mainWriterFile;
	std::queue<std::string> _writerQueue;
	std::vector<std::thread> _writerThreads;
	std::mutex getWriterLock;
	long int _totalWriterThreads;
	long int _pendingWriterThread;
	public:
	Writers(std::string mainWriterFile);
	void processWriterFiles(string fileName);
	void processQueuedWriters();
	void startProcessingWriters(long int n);
	long int getTotalWriterThreads();
	void queueUpWriters();
	void joinWriterThreads();
};

#endif
