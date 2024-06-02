#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <iostream>
#include <string>
using namespace std;


//definition of treeNode with 7 fields.
struct treeNode {
	//has 7 fields 4 pointers ,2 coordiante points and a name.
	treeNode * NWptr;
	treeNode * NEptr;
	treeNode * SWptr;
	treeNode * SEptr;
	int XCOORD;
	int YCOORD;
	string name;

	//constructor 
	treeNode()
	{}

	//parametric constructor v1
	treeNode(treeNode * p1, treeNode * p2, treeNode * p3, treeNode * p4)
		: NWptr(p1), NEptr(p2), SWptr(p3), SEptr(p4)
	{}

	//parametric constructor v2
	treeNode(treeNode * p1, treeNode * p2, treeNode * p3, treeNode * p4, const int & xcor, const int & ycor, const string n)
		: NWptr(p1), NEptr(p2), SWptr(p3), SEptr(p4), XCOORD(xcor), YCOORD(ycor), name(n)
	{}

};
//definition of quadtree class header.


class quadTree{
	private:
		treeNode * root;
		void Insert(const treeNode &city, treeNode *&t) const;
		void makeEmpty(treeNode * &t) const;
		bool isEmpty(treeNode * &t) const;
		void pretty_print(treeNode *&t) const;
		void searchInRadius(const int x, const int y, const int r,int &ctr,string & inCircle, string & visited, treeNode *&t) const;
	public:
		quadTree(); // constructor
		~quadTree(); // destructor.
		void Insert(const treeNode &city); // will insert a city.
		void makeEmpty();
		bool isEmpty();
		void pretty_print();
		//void printInRadius(const int x, const int y, const int r);
		void searchInRadius(const int x, const int y, const int r, int &ctr,string & inCircle, string & visited);


};



#endif