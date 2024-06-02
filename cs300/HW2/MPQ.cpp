/*****************************
the modified priority queue 
class , created by Doruk Benli
*******************************/
#include <iostream>
#include "MPQ.h"
using namespace std;


MPQ::MPQ()
{
	//This is the constructor of the class
	//inserts a dummy to the 0th index so we start with first index.
	// the dummy label is -1.
	// value is used for comparison and label is for identifying.
	item dummy(0,-1); 
	Location.push_back(-1); //for clarification we say that -1 is dummy element, which may represent an empty element on locations
	Array.push_back(dummy);
	currentSize = 0;
}

void MPQ::setLocationSize(int cNum)
{
	//sets the size of the locations array city num +2, since we have dummy
	// i encountered vector subscript out of range error with +1 probably due to dummy var so +2
	Location.resize(cNum+2);
}

MPQ::~MPQ()
{
	//This is the Destructor of the MPQ class.
	Location.clear();
	Array.clear();
}


void MPQ::insert(const int value, const int label)
{
	//item  to be inserted to array.
	item x(value, label);

	int hole = ++currentSize; // index in buildings.
	Array.push_back(x);
	//value of x should be bigger, since this is a max heap.
	for(;hole> 1 && x.value > Array[hole/2].value; hole /= 2)
	{
		//percolate up
		Array[hole] = Array[hole/2];
		Location[Array[hole/2].label] = hole;
	}
	Array[hole] = x;
	Location[label] = hole;
}

void MPQ::percolateDown(int hole)
{
	int child;
	item tmp = Array[hole];
	
	for(; hole*2 <= currentSize; hole = child)
	{
		child = hole*2;
		if(child != currentSize && Array[child+1].value > Array[child].value)
		{
			child++;
		}
		if(Array[child].value > tmp.value)
		{
			//change hole and child and adjust child in locations
			Array[hole] = Array[child]; 
			Location[Array[child].label] = hole;
		}
		else
		{
			break;
		}
	}
	Array[hole] = tmp;
	Location[tmp.label] = hole;
}


int MPQ::getMax()
{
	//returns the max element value currently stored.
	//since this is a max heap, max element is stored at root.
	if(currentSize >= 1)
	{
		return Array[1].value;
	}
	//should be -1 which means dummy var.
	return 0;
}

int MPQ::Remove(int label)
{
	//--algorithm to remove any element in PQ--
	//replace the element labeled with last one.
	//delete the last element
	//percolate down to fix the heap

	if(Location[label] != -1) // empty element then
	{
		int idx = Location[label]; // idx to be removed
		item toRemove = Array[idx]; //element to be removed
		//if i want to remove the last element i just pop back no need to swap
		if(Array[Location[label]].label == Array[currentSize].label)
		{
			Array.pop_back();
			currentSize--;
			//adjust the locations array.
			Location[label] = -1; //a dummy variable
		}
		else
		{
			//if the elemenet i want to remove is not the last element then follow the algorithm above.
			swap(Array[idx], Array[currentSize--]); //swap the variables,
			Array.pop_back();
			if(currentSize == 1)
			{
				//we percolate down if element is not the last one
				Location[Array[1].label] = 1;
			}
			else{
				percolateDown(Location[label]);
			}
			Location[label] = -1; //set location to -1 after removal.
		}
		return toRemove.value;
	}
	return 0;
}

bool MPQ::IsEmpty()
{
	//function that checks if queue is empty or not.
	if(currentSize == 0)
	{
		return true;
	}
	return false;
}