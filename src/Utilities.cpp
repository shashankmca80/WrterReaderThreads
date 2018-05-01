#include "Utilities.h"

using namespace std;

std::vector<std::string> Utilities::split(const std::string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

void Utilities::squeeZeWhiteSpace(string& s)
{
        string::iterator new_end =
        std::unique(s.begin(), s.end(),
        [=](char lhs, char rhs){ return (isspace(lhs) && isspace(rhs)); }
        );
        s.erase(new_end, s.end());
}

void Utilities::fillQueueFromFile(queue<string>& queues, string anyFileName)
{
	if(getFileSize(anyFileName) > 0)
	{
		ifstream file(anyFileName.c_str());
		string line;
		if (file.is_open())
		{
			while (getline(file,line))
				if(getFileSize(line) > 0)
					queues.push(line);
			file.close();
		}
	}
}

off_t Utilities::getFileSize(string fileName)
{
	struct stat st;
	if (stat(fileName.c_str(), &st) == 0)
		return st.st_size;

	return -1; //File does not exist
}

bool Utilities::isValidInputForCacheSize(string cSize)
{
	unsigned int i ;
	for(i = 0 ; i < cSize.length() ; ++i)
	{
		if(!(cSize[i] >= 48 && cSize[i] <= 57))
			return false;
	}
	return true;
}

void Utilities::checkInputValidity(string cacheSize, string writerFile, string inFile)
{
	if(!isValidInputForCacheSize(cacheSize))
	{
		cout<<"Cache Size should be a valid number "<<endl;
		exit(1);
	}
	if((getFileSize(writerFile) <= 0) && (getFileSize(inFile) <= 0))
	{
		cout<<"No valid input for writer file and infile . Atleast one of them should be non-zero to produce some output"<<endl;
		exit(1);
	}
}
