#include <iostream>
#include "FloorPlaner.h"
using namespace std;

int main(int argc, char** argv)
{
	if(argc < 3){cerr << "usage: ./test input.block input.nets" << endl; return 1;}
	FloorPlaner fp;
	fp.readBlock(argv[1]);
	cout << "-----------------------------------" << endl;
	fp.readNets(argv[2]);

	fp.init();
	return 0;
}
