#include <iostream>
#include <vector>
#include <string>
#include "HashTable.h"

using namespace std;


HashTable::HashTable(string & notFound, int size) : ITEM_NOT_FOUND(notFound), currentSize(size)
{
	// my size is 4099 since i can have 4096 different combinations of code and 4099 is the next prime after that.
	Array.resize(size);
	makeEmpty();
	for(int i= 0; i < 256; i++)
	{
		Array[i].info = ACTIVE;
		Array[i].K = char(i); // initialize the array.
		Array[i].C = i;
	}
}

HashTable::~HashTable()
{
	// so we delete all the values and make empty the whole array.
	Array.clear();
	makeEmpty(); 
}


int HashTable::hash(const string &code) const
{
	int sum = 0;

	for(int i = 0; i < code.length(); i++)
	{
		sum = sum + code[i];
	}

	return sum % currentSize;
}

void HashTable::makeEmpty(){

	for(int i = 0; i < Array.size(); i++)
	{
		Array[i].info = EMPTY;
	}
}

int HashTable::findPos(const string & x)const
{
	//perform a linear probing as its stated in document.

	int i = 0; // collusion count
	int p = hash(x); //current position
	while(Array[p].info != EMPTY && Array[p].K != x)
	{
		// we increment i, which is the num of collusions and add it to pos.
		p++;
		if(p >= Array.size())
		{
			//perform the mod
			p -= Array.size();
		}
	}
	return p;
}

bool HashTable::isActive(int currentPos) const
{
	return Array[currentPos].info == ACTIVE;
}

void HashTable::remove(const string & x)
{
	int currentPos = findPos(x);
	if(isActive(currentPos))
	{
		Array[currentPos].info = DELETED;
	}

}

string HashTable::find(const string & x) const
{
	int currentPos = findPos(x);

	if(isActive(currentPos))
	{
		return Array[currentPos].K;
	}
	return ITEM_NOT_FOUND;
}

void HashTable::insert(const string & x)
{
	//insert x as active
	int currentPos = findPos(x);
	if(isActive(currentPos))
	{
		return; //do nothing
	}
	Array[currentPos] = HashEntry(x,currentPos, ACTIVE);


}
