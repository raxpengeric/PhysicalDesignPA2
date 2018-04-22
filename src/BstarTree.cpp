#include <iostream>
#include <vector>
#include <list>
#include <cfloat>
#include <string>
#include "FloorPlaner.h"
#include "BstarTree.h"
using namespace std;

BTree::BTree(vector<Block*>& BlockList){
	Xmax = Ymax = 0;
	NodeList.reserve(BlockList.size());
	BTreeNode* lastNode = NULL;
	for(vector<Block*>::iterator it = BlockList.begin(); it != BlockList.end(); ++it){
		BTreeNode* Nptr = new BTreeNode(*it, lastNode);
		if(lastNode == NULL){
			_root = Nptr;
		}else{
			lastNode->_lchild = Nptr;
		}
		NodeList.push_back(Nptr);
		lastNode = Nptr;
	}
	if(NodeList.size() != BlockList.size()){
		cerr << "Warning: NodeList.size() != BlockList.size()" << endl;
	}
	ContourLine.push_back(ContourNode(0,DBL_MAX,0));
}

double BTree::updateContour(BTreeNode* Node){	// update region (x,X) return y
	double y = 0, x = Node->getX(), X = x+Node->getwidth();
	list<ContourNode>::iterator it = ContourLine.begin();
	while(1){
		if(it->X <= x){
			++it;
		}else if(it->x >= X){
			break;
		}else{
			if(it->Y > y) y = it->Y;
			if(it->x >= x && it->X <= X){
				ContourLine.erase(it);
			}else if( it->x <= x && it->X <= X){
				it->X = x;
				++it;
			}else if( it->x >= x && it->X >= X){
				it->x = X;
				break;
			}else{
				ContourLine.insert(it,ContourNode(it->x,x,it->Y));
				it->x = X;
				break;
			}
		}
	}
	if(y+Node->getheight() > Ymax) Ymax = y+Node->getheight();
	ContourLine.insert(it,ContourNode(x,X,y+Node->getheight()));
	return y;
}

void BTree::Pack(BTreeNode* Node){
	if(Node == NULL){
		Node = _root;
		Node->setX(0);
		Node->setY(0);
		ContourLine.push_front(ContourNode(0,Node->getwidth(),Node->getheight()));
		Ymax = Node->getheight();
	}else if(Node == Node->_parent->_lchild){
		double x = Node->_parent->getX()+Node->_parent->getwidth();
		Node->setX(x);
		if(x+Node->getwidth() > Xmax) Xmax = x+Node->getwidth();
		Node->setY(updateContour(Node));
	}else if(Node == Node->_parent->_rchild){
		double x = Node->_parent->getX();
		Node->setX(x);
		if(x+Node->getwidth() > Xmax) Xmax = x+Node->getwidth();
		Node->setY(updateContour(Node));
	}else{
		return;
		//cerr <<"Pack("<<Node->_block->getname()<<"): This shouldn't happened!!"<< endl;
	}
	if(Node->_lchild != NULL) Pack(Node->_lchild);
	else if(Node->_rchild != NULL) Pack(Node->_rchild);
}

void BTree::printPacking(){
	cout << "Bounding Box: (" << Xmax << ',' << Ymax << ")" << endl;
	cout << "Packing location:" << endl;
	vector<BTreeNode*>::iterator it = NodeList.begin();
	for(; it != NodeList.end(); ++it){
		string name = (*it)->_block->getname();
		cout << name << ": " << (*it)->getX() << ',' << (*it)->getY() << endl;
	}
}
























