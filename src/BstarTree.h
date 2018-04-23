#ifndef _BSTARTREE_H_
#define _BSTARTREE_H_
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include "cirType.h"
#include "FloorPlaner.h"
using namespace std;


class ContourNode{
friend class BTree;
private:
	ContourNode(double _x, double _X, double _Y):
		x(_x), X(_X), Y(_Y){;}
	double x, X, Y;
};

class BTreeNode{
friend class BTree;
public:
	BTreeNode(Block* block, BTreeNode* parent): 
		_block(block), _parent(parent), _lchild(NULL), _rchild(NULL), rotate(false){;}
	void setX(double X){_block->setX(X);} double getX(){_block->getX();}
	void setY(double Y){_block->setY(Y);} double getY(){_block->getY();}
	
	double getwidth() {return (rotate)?_block->getheight():_block->getwidth();}
	double getheight(){return (rotate)?_block->getwidth():_block->getheight();}


private:
	BTreeNode *_parent, *_lchild, *_rchild;
	Block*     _block;
	bool       rotate;
};

class BTree{
public:
	BTree(vector<Block*>& BlockList);
	~BTree(){clear();}
	void build(vector<Block*>& BlockList);
	void clear(){for(size_t i = 0; i < NodeList.size(); ++i) delete NodeList.at(i);}

	// Perturbation
	bool Pertubation();
	bool Rotate(BTreeNode* node);
	bool DelnIns(BTreeNode* del, BTreeNode* ins);
	bool Swap(BTreeNode* node1, BTreeNode* node2);
	BTreeNode* getNode(string name){ return (NodeMap.find(name) == NodeMap.end())? NULL : NodeMap[name];}
	
	// Packing
	double updateContour(BTreeNode* Node);
	void Pack(BTreeNode* node=NULL);
	void printPacking();

private:
	vector<BTreeNode*> NodeList;
	map<string, BTreeNode*> NodeMap;

	double Xmax, Ymax;
	list<ContourNode> ContourLine;
	BTreeNode* _root;
};


#endif
