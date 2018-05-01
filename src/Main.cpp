#include "Main.h"
using namespace std;

int main(int argc, char **argv)
{
	if(argc < 5)
	{
		cout<<"Usage : ./cache cache_size readerFile WriterFile Infile"<<endl;
		exit(-1);
	}
	ThreadsHandler th(argv[1], argv[2], argv[3], argv[4]);
	th.startProcess();
	return 0;	
}
