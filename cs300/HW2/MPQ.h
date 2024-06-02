/*****************************
the modified priority queue 
class , created by Doruk Benli
*******************************/
#ifndef _MPQ_H_
#define _MPQ_H_
#include <iostream>
#include <vector>
using namespace std;


struct item{

	int value; //just value for comparison(y)
	int label; // unique identifier
	item();
	item(int v, int l) :value(v), label(l)
	{};

};

class MPQ{
	private:
		int currentSize; //number of elements in heap
		vector<item> Array; // the heap array
		void percolateDown(int hole);
		vector<int> Location;
	public:
		MPQ(); //constructor
		~MPQ(); //destructor
		void insert(const int value, const int label); //inserts item with given value and label
		int Remove(int label); // removes the value with given label
		int getMax(); //returns max value
		bool IsEmpty(); // checks if its empty.
		void setLocationSize(int cNum);
};



#endif