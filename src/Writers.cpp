#include "Writers.h"

using namespace std;

Writers::Writers(string mainWriterFile) : _mainWriterFile(mainWriterFile)
{
	queueUpWriters();
	_totalWriterThreads = _writerQueue.size();	
}

void Writers::processWriterFiles(string fileName)
{
	cout<<"Writer Processing file "<<fileName<<endl;
	ifstream file(fileName.c_str());
	string line;
	if (file.is_open())
	{
		while (getline(file,line))
		{
			Utilities::squeeZeWhiteSpace(line);			
			vector<string>getRecord = Utilities::split(line, ' ');			
			if((getRecord.size() == 2) && (stol(getRecord[0]) > 0))
				ThreadsHandler::updateDiskContentsAndCache(getRecord[0], getRecord[1]);
		}
		file.close();
	}
}


void Writers::processQueuedWriters()
{
	string writerFileName;
	getWriterLock.lock();
	writerFileName = _writerQueue.front();
	_writerQueue.pop();
	processWriterFiles(writerFileName);
	getWriterLock.unlock();
}

void Writers::startProcessingWriters(long int n)
{
	for(auto i = 0 ; i < n ; ++i)
		_writerThreads.push_back(thread([=] { processQueuedWriters() ; } ));	
}
long int Writers::getTotalWriterThreads()
{
	return _totalWriterThreads ;
}
void Writers::queueUpWriters()
{
	Utilities::fillQueueFromFile(_writerQueue, _mainWriterFile);
}

void Writers::joinWriterThreads()
{
	for(auto &t : _writerThreads)
		t.join();
}
