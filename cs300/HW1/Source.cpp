#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "QuadTree.h"

using namespace std;


int main(){
	 string fname1, fname2, line;
	 ifstream cities, queries;
	 int s1 , s2, iter=0; //s1 and s2 are limits
	 int x, y, r;
	 quadTree tree1;

	 // opening the files cities and querries
	 fname1 = "cities.txt";
	 fname2 = "queries.txt";
	 cities.open(fname1.c_str());
	 queries.open(fname2.c_str());
	 treeNode insertNode;
	 while(getline(cities,line))
	 {
		 if(iter == 0)
		 {
			 int idx = line.find(' ');
			 s1 = stoi(line.substr(0,idx));
			 s2 = stoi(line.substr(idx+1));
		 }
		 else
		 {
			 int sidx = line.find(' ');
			 string Name = line.substr(0,sidx);
			 line = line.substr(sidx+1);
			 int i_idx = line.find(' ');
			 int xcor = stoi(line.substr(0,i_idx));
			 int ycor = stoi(line.substr(i_idx+1));
			 insertNode.name = Name;
			 insertNode.XCOORD =xcor;
			 insertNode.YCOORD = ycor;
			 tree1.Insert(insertNode);
		 }
		 iter += 1;
	 }

	 tree1.pretty_print();
	 cout << endl;
	
	 line.clear();
	 while(getline(queries,line))
	 {
		 string inCircle ="";
		 string visited = "";
		 int ctr =0;
		 int commaidx =line.find(',');
		 x = stoi(line.substr(0,commaidx));
		 line = line.substr(commaidx+1);
		 int commaidx2 =line.find(',');
		 y = stoi(line.substr(0,commaidx2));
		 line = line.substr(commaidx2+1);
		 r = stoi(line);
		 cout << endl;
		 tree1.searchInRadius(x,y,r,ctr,inCircle,visited);
		 if(ctr == 0)
		 {
			 cout << "<None>" << endl;
		 }
		 else
		 {
			 int idx = inCircle.rfind(',');
			 cout << inCircle.substr(0,idx) << endl;
		 }
		 int lidx = visited.rfind(',');
		 cout << visited.substr(0,lidx) << endl;
	 }
	 


	return 0;
}