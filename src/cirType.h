#ifndef _CIRTYPE_H_
#define _CIRTYPE_H_

#include <iostream>
#include <string>
#include <vector>
using namespace std;


class Terminal{
public:
	Terminal(string name, double X=0, double Y=0):
		_name(name), _X(X), _Y(Y){;}
	string getname(){return _name;}
	void setX(double x){_X = x;} double getX(){return _X;}
	void setY(double y){_Y = y;} double getY(){return _Y;}
	
	void print(){ cout << _name << " terminal " << _X << ' ' << _Y << endl;}
protected:
	string _name;
private:
	double _X, _Y;
};

class Block: public Terminal{
public:
	Block(string name, double width, double height):
		Terminal(name), _width(width), _height(height){;}
	void setX(double x){_X = x;static_cast<Terminal*>(this)->setX(_X + _width/2);} double getX(){return _X;}
	void setY(double y){_Y = y;static_cast<Terminal*>(this)->setY(_Y + _width/2);} double getY(){return _Y;}

	double getwidth() { return _width;}
	double getheight(){ return _height;}
	
	void print(){ cout << _name << ' ' << _width << ' ' << _height << endl;}
private:
	double _width, _height;
	double _X, _Y;
};

class Net{
public:
	void addTerminal(Terminal* t){_terminalList.push_back(t);}
	void update(Terminal* t){ 
		if(t->getX() > X) X = t->getX(); else if(t->getX() < X) x = t->getX();
		if(t->getY() > Y) Y = t->getY(); else if(t->getY() < Y) y = t->getY();
	}
	size_t degree(){return _terminalList.size();}
	void printTerminals(){
		for(size_t i = 0; i < _terminalList.size(); ++i){
			cout << _terminalList[i]->getname() << endl;
		}
	}
private:
	double x, X, y, Y;					// bounding Box coordinate
	vector<Terminal*> _terminalList;
};


#endif
