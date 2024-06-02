#ifndef _HASHTABLE_H
#define _HASHTABLE_H
#include <iostream>
#include <vector>
#include <string>

using namespace std;
// will take string as key and convert it to int which is code.
//hash table for encoding. i.e compress.
class HashTable{
	public:
		HashTable(string & notFound, int size); //setting size to 256 as mentioned in document
		~HashTable(); //destrcutor 
		void makeEmpty();
		void insert(const string & x);
		void remove(const string & x);
		string find(const string & x) const;
		int findPos(const string & x)const;
		enum EntryType{ACTIVE, EMPTY, DELETED};

	private:
		struct HashEntry{ //this will act as my hash table cell
			string K;
			int C;
			EntryType info;
			HashEntry(){};
			HashEntry(const string & e, int c, EntryType i=EMPTY): K(e), C(c),info(i){}

		};
		vector<HashEntry> Array; //this will act as my hash table
		int currentSize;
		const string ITEM_NOT_FOUND;
		bool isActive(int currentPos) const;
		int hash(const string & code) const;
};

#endif