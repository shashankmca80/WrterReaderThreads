#include "HandleThreads.h"
#include "Readers.h"
#include "Writers.h"

using namespace std;

shared_timed_mutex ThreadsHandler::_cacheLock;
shared_timed_mutex ThreadsHandler::_diskLock;
shared_timed_mutex ThreadsHandler::_cacheOrDiskLock;
shared_timed_mutex ThreadsHandler::_createFileLock;

vector<string> ThreadsHandler::_infoFileContentsOnDisk = vector<string>(DISKSIZE);
MemCache ThreadsHandler::_memCache;
CachedItemsList ThreadsHandler::_cachedItemsTracker;
C_LINT ThreadsHandler::_inFileSize = 0;
C_UINT ThreadsHandler::_cacheSize = 0;

ThreadsHandler::ThreadsHandler(string cacheSize, string readerFile, string writerFile, string inFile) : _inputCacheSize(cacheSize), _readerFile(readerFile), 
													_writerFile(writerFile), _inFile(inFile)
{
	_readers = make_shared<Readers>(_readerFile);
	_writers = make_shared<Writers>(_writerFile);
}

string ThreadsHandler::readRecordFromCacheOrDisk(string lineNum)
{
	shared_lock<std::shared_timed_mutex> readersLock(_cacheOrDiskLock);
	if((_memCache.size() > 0) && (_memCache.find(lineNum) != _memCache.end()))
		return (_memCache[lineNum]+" Cache");
	else if(((stol(lineNum)-1) >= 0) && ((stol(lineNum)-1) < _inFileSize))
		return (_infoFileContentsOnDisk[stol(lineNum)-1] + " Disk");
	return "Invalid Record";
}

void ThreadsHandler::updateDiskContentsAndCache(string lineNum, string contents)
{
	lock_guard<std::shared_timed_mutex> writersLock(_cacheOrDiskLock);

	//To avoid issues becaue of comparison between signed and unsigned
	C_LINT diskSize = _infoFileContentsOnDisk.size();
	if((stol(lineNum) >= 0) && (_inFileSize < diskSize))
	{
		_infoFileContentsOnDisk[stol(lineNum)-1] = contents;
		_inFileSize = max(_inFileSize, (stol(lineNum)-1));
		if(_cacheSize == _memCache.size())
		{
			_memCache.erase(_cachedItemsTracker.back());
			_memCache[lineNum] = contents;
			_cachedItemsTracker.pop_back();
			_cachedItemsTracker.push_front(lineNum);
		}
		else
		{
			_memCache[lineNum] = contents;
			_cachedItemsTracker.push_front(lineNum);
		}
	}
}

void ThreadsHandler::createOrOverWriteinFile(vector<string>& v, string inFileName)
{
	fstream out;
	out.open(inFileName.c_str(), ios::out | ios::trunc);
	long int c ;
	for(c = 0 ; c <= _inFileSize ; ++c)
	{
		if(((c == _inFileSize) && (v[c].length() > 0)) || (c < _inFileSize))
		{
			out<<v[c];
			out<<"\n";
		}
	}	
	out.close();
	v.erase(v.begin(), v.end());
	cout<<"infile "<<inFileName<<" Created : "<<endl;
}

void ThreadsHandler::createReaderFileFromVector(vector<string>& vec,string name)
{
	fstream out;
	out.open(name.c_str(), ios::out | ios::trunc);
	for(auto lineNo : vec) 
	{
		string getRecord = readRecordFromCacheOrDisk(lineNo); 
		out<<getRecord;
		out<<"\n";
	}
	out.close();
	vec.erase(vec.begin(), vec.end());
	cout<<"File : "<<name<<" Created"<<endl;
}

void ThreadsHandler::fillVectorFromFile(vector<string>& vec,string name, bool isInfile/*=false*/)
{
	ifstream file(name.c_str());
	string line;
	if (file.is_open())
        {
                while (getline(file,line))
			if(isInfile)
			{
				vec[_inFileSize] = line;			
				++_inFileSize;
			}
			else
				vec.push_back(line);
		file.close();
        }
}

void ThreadsHandler::startProcess()
{
	Utilities::checkInputValidity(_inputCacheSize, _writerFile, _inFile);
	_cacheSize = stoul(_inputCacheSize);
	fillVectorFromFile(_infoFileContentsOnDisk, _inFile, true);

	long int readerThreads = _readers->getTotalReaderThreads();
	long int halfOfReaderThreads = readerThreads / 2;
	long int pendingReaders = readerThreads - halfOfReaderThreads ;
	long int writerThreads = _writers->getTotalWriterThreads();
	long int halfOfWriterThreads = writerThreads / 2;
	long int pendingWriters = writerThreads - halfOfWriterThreads;

	if((Utilities::getFileSize(_inFile) <= 0) && (writerThreads == 0))
	{
		cout<<"There are no writer threads to run and also infile is either empty or does not exist. "<<endl;
		exit(1);
	}
	_readers->startProcessingReaders(halfOfReaderThreads);
	_writers->startProcessingWriters(halfOfWriterThreads);
	_readers->startProcessingReaders(pendingReaders);
	_writers->startProcessingWriters(pendingWriters);
	_readers->joinReaderThreads();
	_writers->joinWriterThreads();

	cout<<"All Threads Done !!!"<<endl;
	createOrOverWriteinFile(_infoFileContentsOnDisk, _inFile);
}
