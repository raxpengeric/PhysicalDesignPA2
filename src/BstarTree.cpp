#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <cfloat>
#include <string>
#include "FloorPlaner.h"
#include "BstarTree.h"
using namespace std;

BTree::BTree(vector<Block*>& BlockList){build(BlockList);}

void BTree::build(vector<Block*>& BlockList){
	Xmax = Ymax = 0;
	NodeList.reserve(BlockList.size());
	BTreeNode* lastNode = NULL;
	for(vector<Block*>::iterator it = BlockList.begin(); it != BlockList.end(); ++it){
		BTreeNode* Nptr = new BTreeNode(*it, lastNode);
		if(lastNode == NULL){
			_root = Nptr;
		}else{
			lastNode->_rchild = Nptr;
		}
		NodeList.push_back(Nptr);
		NodeMap[Nptr->_block->getname()] = Nptr;
		lastNode = Nptr;
	}
	if(NodeList.size() != BlockList.size()){
		cerr << "Warning: NodeList.size() != BlockList.size()" << endl;
	}
	ContourLine.push_back(ContourNode(0,DBL_MAX,0));
}

bool BTree::Rotate(BTreeNode* node){
	if(node == NULL) return false;
	else{
		node->rotate = !node->rotate;
		return true;
	}
}

bool BTree::DelnIns(BTreeNode* del, BTreeNode* ins){
	if(del == ins || del == NULL || ins == NULL) return false;			// check false input
	else if(ins->_lchild != NULL && ins->_rchild != NULL) return false;	// able to insert
	else{
cerr << "Enter DelnIns..." << endl;
		// delete node
		if(del->_lchild != NULL && del->_rchild != NULL){
			if(del->_lchild->_rchild != NULL && del->_rchild->_lchild != NULL) return false; // unable to delete
			else if(del->_lchild->_rchild == NULL){
				del->_lchild->_parent = del->_parent;
				if(del->_parent->_lchild == del) del->_parent->_lchild = del->_lchild;
				else del->_parent->_rchild = del->_lchild;
				del->_rchild->_parent = del->_lchild;
				del->_lchild->_rchild = del->_rchild;
				del->_rchild = NULL;
				del->_lchild = NULL;
			}else if(del->_rchild->_lchild == NULL){
				del->_rchild->_parent = del->_parent;
				if(del->_parent->_lchild == del) del->_parent->_lchild = del->_rchild;
				else del->_parent->_rchild = del->_rchild;
				del->_lchild->_parent = del->_rchild;
				del->_rchild->_lchild = del->_lchild;
				del->_rchild = NULL;
				del->_lchild = NULL;
			}
		}else if(del->_lchild != NULL){
cerr << "removing left child" << endl;
			del->_lchild->_parent = del->_parent;
			if(del->_parent->_lchild == del) del->_parent->_lchild = del->_lchild;
			else del->_parent->_rchild = del->_lchild;
			del->_lchild = NULL;
		}else if(del->_rchild != NULL){
			del->_rchild->_parent = del->_parent;
			if(del->_parent->_lchild == del) del->_parent->_lchild = del->_rchild;
			else del->_parent->_rchild = del->_rchild;
			del->_rchild = NULL;
		}else{
			if(del->_parent->_lchild == del) del->_parent->_lchild = NULL;
			else del->_parent->_rchild = NULL;
		}
		// insert node
		if(ins->_lchild == NULL && ins->_rchild == NULL){
			if(rand()&1) ins->_lchild = del;
			else ins ->_rchild = del;
		}else if(ins->_lchild == NULL){
			ins->_lchild = del;
		}else if(ins->_rchild == NULL){
			ins->_rchild = del;
		}else cerr << "Insert Error!!" << endl;
		del->_parent = ins;
		return true;
	}

}

bool BTree::Swap(BTreeNode* node1, BTreeNode* node2){
	if(node1 == NULL || node2 == NULL){return false;}
	// swap node information only
	Block* temp = node1->_block;
	node1->_block = node2->_block;
	node2->_block = temp;
	bool rt = node1->rotate;
	node1->rotate = node2->rotate;
	node2->rotate = rt;
	return true;
}


double BTree::updateContour(BTreeNode* Node){					// update region (x,X) return y
	double Y, y = 0, x = Node->getX(), X = x+Node->getwidth();
	list<ContourNode>::iterator it = ContourLine.begin();
	while(it != ContourLine.end()){
		if(it->X <= x){
			++it;
		}else if(it->x >= X){
			break;
		}else{
			if(it->Y > y) y = it->Y;
			if(it->x >= x && it->X <= X){
				it = ContourLine.erase(it);
			}else if( it->x <= x && it->X <= X){
				it->X = x; break;
			}else if( it->x >= x && it->X >= X){
				it->x = X; break;
			}else{
				ContourLine.insert(it,ContourNode(it->x,x,it->Y));
				it->x = X; break;
			}
		}
	}
	Y = y+Node->getheight();
	if(Y > Ymax) Ymax = Y;
	ContourLine.insert(it,ContourNode(x,X,Y));
	return y;
}

void BTree::Pack(BTreeNode* Node){
	if(Node == NULL && _root != NULL){
		ContourLine.clear();
		Node = _root;
		Node->setX(0);
		Node->setY(0);
		ContourLine.push_back(ContourNode(0,Node->getwidth(),Node->getheight()));
		ContourLine.push_back(ContourNode(Node->getwidth(),DBL_MAX,0));
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
		cerr <<"Pack("<<Node->_block->getname()<<"): This shouldn't happened!!"<< endl;
		return;
	}
	if(Node->_lchild != NULL) Pack(Node->_lchild);
	if(Node->_rchild != NULL) Pack(Node->_rchild);
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
























