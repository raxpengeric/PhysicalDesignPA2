/*****************************************************************************************************/
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include "FloorPlaner.h"
#include "BstarTree.h"
using namespace std;

#ifdef DEBUG
#define DEBUG_readBlock
#define DEBUG_readNets
#define DEBUG_pack
#endif

static const char* WHITE_SPACE = " \t\n\r\v\f";

void FloorPlaner::readBlock(const string& filename){
	size_t begin, end, width, height; char* pEnd;
	ifstream ifile(filename.c_str());
	string line, name;
	while(getline(ifile,line)){
		if(line.find_first_not_of(WHITE_SPACE) == string::npos) continue;
		end = line.find_first_of(WHITE_SPACE, 0);
		name = line.substr(0,end);
		if(name == "Outline:"){
			outWidth  = strtod(line.c_str()+end,&pEnd);
			outHeight = strtod(pEnd,&pEnd);
		}else if(name == "NumBlocks:"){
			NumBlocks = strtod(line.c_str()+end,NULL);
			BlockList.reserve(NumBlocks);
		}else if(name == "NumTerminals:"){
			NumTerminals = strtod(line.c_str()+end,NULL);
		}else{
			begin = line.find_first_not_of(WHITE_SPACE, end);
			end = line.find_first_of(WHITE_SPACE, begin);
			if(line.substr(begin, end-begin) == "terminal"){
				width  = strtod(line.c_str()+end, &pEnd);
				height = strtod(pEnd, &pEnd);
				TerminalMap[name] = new Terminal(name, width, height);
			}else{
				width  = strtod(line.c_str()+begin, &pEnd);
				height = strtod(pEnd, &pEnd);
				Block* Bptr = new Block(name, width, height);
				BlockMap[name] = Bptr;
				BlockList.push_back(Bptr);
				TerminalMap[name] = static_cast<Terminal*>(Bptr);
			}
		}
	}
	ifile.close();

	// check NumBlocks NumTerminal
	if(NumBlocks != BlockMap.size())      { cerr << "warning: NumBlocks != BlockMap.size()" << endl;}
	if((NumTerminals+NumBlocks) != TerminalMap.size()){ cerr << "warning: NumTerminals != TerminalMap.size()" << endl;}
#ifdef DEBUG_readBlock
cout << "Outline: " << outWidth << ' ' << outHeight << endl;
cout << "NumBlocks: " << NumBlocks << endl;
cout << "NumTerminals: " << NumTerminals << endl;
for(auto it = BlockMap.begin(); it != BlockMap.end(); ++it)
	it->second->print();
for(auto it = TerminalMap.begin(); it != TerminalMap.end(); ++it)
	if(BlockMap.find(it->second->getname()) == BlockMap.end())it->second->print();
#endif
}

void FloorPlaner::readNets(const string& filename){
	size_t begin, end, degree; char* pEnd;
	ifstream ifile(filename.c_str());
	string line, name;
	while(getline(ifile,line)){
		if(line.find_first_not_of(WHITE_SPACE) == string::npos) continue;
		end = line.find_first_of(WHITE_SPACE, 0);
		name = line.substr(0,end);
		if(name == "NumNets:"){
			NumNets = strtoul(line.c_str()+end,NULL,10);
		}else if(name == "NetDegree:"){
			degree = strtoul(line.c_str()+end,NULL,10);
			Net* Nptr = new Net;
			for(size_t d = 0; d < degree; ++d){
				getline(ifile, line);
				end = line.find_first_of(WHITE_SPACE, 0);
				name = (end == string::npos) ? line.substr(0,end) : line;
				if(TerminalMap.find(name) != TerminalMap.end()){
					Nptr->addTerminal(TerminalMap.at(name));
				}else{
					cerr << "read Net Name error: " << name << endl;
				}
			}
			NetList.push_back(Nptr);
		}
	}
	ifile.close();
	if(NumNets != NetList.size()){cerr << "warning: NumNets != NetList.size()" << endl;}
#ifdef DEBUG_readNets
cout << "NumNets: " << NetList.size() << endl;
for(size_t i = 0; i < NetList.size(); ++i){
	cout << "NetDegree: " << NetList[i]->degree() << endl;
	NetList.at(i)->printTerminals();
}
#endif
}

void FloorPlaner::init(){
	tree = new BTree(BlockList);
	tree->Pack();
#ifdef DEBUG_pack
tree->printPacking();
#endif
	cout << "--------------------------------------------" << endl;
	if(!tree->DelnIns(tree->getNode("C"), tree->getNode("A"))) cerr << "DelnIns error!!" << endl;
	if(!tree->DelnIns(tree->getNode("D"), tree->getNode("B"))) cerr << "DelnIns error!!" << endl;
	tree->Swap(tree->getNode("B"),tree->getNode("C"));
	//tree->Rotate(tree->getNode("B"));
	//if(!tree->DelnIns(tree->getNode("D"), tree->getNode("A"))) cerr << "DelnIns error!!" << endl;
	tree->Pack();
	tree->printPacking();
}

/*****************************************************************************************************/






















