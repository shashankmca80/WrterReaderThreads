#include "Readers.h"

using namespace std;

Readers::Readers(std::string mainReaderFile) : _mainReaderFile(mainReaderFile)
{
	queueUpReaders();
	_totalReaderThreads = _readerQueue.size();
}

void Readers::processQueuedReaders()
{
	string gotfileName ;
	vector<string> contents;
	getReaderLock.lock();
	gotfileName = _readerQueue.front();
	_readerQueue.pop();
	ThreadsHandler::fillVectorFromFile(contents, gotfileName, false);
	gotfileName = gotfileName+".output.txt";
	ThreadsHandler::createReaderFileFromVector(contents, gotfileName);
	getReaderLock.unlock();

}
void Readers::startProcessingReaders(long int n)
{
	for(auto i = 0 ; i < n ; ++i)
		_readerThreads.push_back(thread([=] { processQueuedReaders() ; } ));	
}
long int Readers::getTotalReaderThreads()
{
	return _totalReaderThreads ;
}
void Readers::queueUpReaders()
{
	Utilities::fillQueueFromFile(_readerQueue, _mainReaderFile);
}

void Readers::joinReaderThreads()
{
	for(auto &t : _readerThreads)
		t.join();
}
