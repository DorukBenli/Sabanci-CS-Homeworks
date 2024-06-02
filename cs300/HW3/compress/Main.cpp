#include <iostream>
#include <fstream>
#include "HashTable.h"

//the problem is i do not insert b
 
using namespace std;
int main()
{
	string check;
	string notFound = "ITEM_NOT_FOUND", message;
	char ch;
	ifstream filein("compin.txt");
	ofstream fileout("compout.txt");

	//create the hash table object.
	HashTable table(notFound, 4099);

	while(!filein.eof())
	{	
		filein.get(ch);
		cout << ch << endl;
		message+=ch;
	}
	// if find returns ITEM_NOT_FOUND as a string this means we do not have that string sequence inside it.
	// insert p + c to dictionary
	// iterating through whole string is not logical due to fact that i can insert m
	
	int idx_p = 0;
	int idx_c = 1;
	string p,c;
	p = message[0];

	
	
	while(idx_p + idx_c <= message.size())
	{
		c = message[idx_c];
		if(table.find(p) != notFound)
		{
			//if we have the entry in the table
			p = message.substr(idx_p, idx_c+1);
			idx_c++;
		}
 		else
		{
			//if we do not have the entry in table
			fileout<< table.findPos(p);
			table.insert(p);
			idx_p += idx_c-1;
		}

	}

	fileout.close();
	filein.close();
	return 0;
}