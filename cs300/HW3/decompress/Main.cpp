#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "HashTable.h"
using namespace std;

int main()
{
	ifstream filein("compout.txt");
	ofstream fileout("decompout.txt");
	string line;
	istringstream ss;
	vector<int> codes;
	string notFound = "ITEM_NOT_FOUND";
	HashTable table(notFound, 4099);
	getline(filein, line);
	int i = 0;

	//putting the codes to my vector through iterating on a single line.
	while(line[i] != '\0')
	{
		if(line[i] != ' ')
		{
			i++;
		}
		else{
			int num = stoi(line.substr(0,i));
			codes.push_back(num);
			line = line.substr(i+1);
			i=0;
		}
	}

	for(int i= 0; i< codes.size(); i++)
	{
		//i code is in my dictionary then i write it to file.
		// and add prev code and this texts first letter.
		int encoded = codes[i];
		string info = table.getcode(encoded);
		if(table.find(info) != notFound)
		{
			if(i == 0)
			{
				fileout<< table.getcode(encoded);
				string text = table.getcode(encoded) + table.getcode(encoded)[0]; //adding the previous code and currents code first letter
				table.insert(text);
				cout << text << endl;
			}
			else
			{
				int prev = codes[i-1];
				string text = table.getcode(prev) + table.getcode(encoded)[0]; //adding the previous code and currents code first letter
				fileout<< table.getcode(encoded);
				table.insert(text);
				cout << text << endl;
			}
			
		}
		else
		{
			int prev = codes[i-1];
			string text = table.getcode(prev) + table.getcode(prev)[0];
			fileout << text;
			table.insert(text);
			cout << text << endl;
		}
	}





	filein.close();
	fileout.close();
	return 0;
}