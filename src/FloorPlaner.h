#ifndef _FLOORPLANER_H_
#define _FLOORPLANER_H_
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "cirType.h"
#include "BstarTree.h"
using namespace std;

class FloorPlaner{
public:
	void readBlock(const string& filename);
	void readNets(const string& filename);
	void init();


private:
	double outWidth, outHeight;
	size_t NumBlocks, NumTerminals, NumNets;
	//unordered_map<string, Block*> BlockMap;
	//unordered_map<string, Terminal*> TerminalMap;
	map<string, Block*> BlockMap;
	map<string, Terminal*> TerminalMap;

	vector<Block*> BlockList;

	vector<Net*> NetList;
	
	BTree* tree;
};











#endif
