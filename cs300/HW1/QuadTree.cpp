#include <iostream>
#include <string>
#include "QuadTree.h"
using namespace std;

quadTree::quadTree()
{
	// constructor will set roots pointers to null and coordinates to null aswell.
	root = NULL;
}

void quadTree::Insert(const treeNode &toBeInserted)
{
	//public insert
	Insert(toBeInserted, root);
}

void quadTree::Insert(const treeNode &toBeInserted, treeNode * &t) const
{
	// this is the private ýnsert
	//t is root or the reference point
	if(t==NULL)
	{
		//makes to be inserted the root, i.e creates a new node
		//this will make every insert operation and the recursive call will terminate once this is executed.
		t=new treeNode(NULL,NULL,NULL,NULL,toBeInserted.XCOORD,toBeInserted.YCOORD,toBeInserted.name);
	}
	else if(toBeInserted.XCOORD < t->XCOORD && toBeInserted.YCOORD < t->YCOORD )
	{
		// insert to south west
		Insert(toBeInserted, t->SWptr);
	}
	else if(toBeInserted.XCOORD >= t->XCOORD && toBeInserted.YCOORD < t->YCOORD)
	{
		//insert to south east
		Insert(toBeInserted, t->SEptr);
	}
	else if(toBeInserted.XCOORD < t->XCOORD && toBeInserted.YCOORD >= t->YCOORD)
	{
		//insert to north west
		Insert(toBeInserted, t->NWptr);
	}
	else if(toBeInserted.XCOORD >= t->XCOORD && toBeInserted.YCOORD >= t->YCOORD)
	{
		//insert to north east
		Insert(toBeInserted, t->NEptr);
	}
	else
	{
		; // duplicate do nothing
	}
	

}

void quadTree::makeEmpty()
{
	makeEmpty(root);
}

void quadTree::makeEmpty(treeNode * &t) const
{
	// private make empty function.
	if(t != NULL)
	{
		makeEmpty(t->SWptr);
		makeEmpty(t->SEptr);
		makeEmpty(t->NWptr);
		makeEmpty(t->NEptr);
		delete t;
	}
	t= NULL;


}

quadTree::~quadTree()
{
	// destructor which calls make empty function.
	makeEmpty();
}

bool quadTree::isEmpty()
{
	return isEmpty(root);
}

bool quadTree::isEmpty(treeNode * &t) const 
{
	if(t == NULL)
	{
		return true;
	}
	return false;
}

void quadTree::pretty_print()
{
	pretty_print(root);
}

void quadTree::pretty_print(treeNode *&t) const
{
	if (t != NULL)
	{
		cout << t->name << endl;
		pretty_print(t->SEptr);  
		pretty_print(t->SWptr);  
		pretty_print(t->NEptr);  
		pretty_print(t->NWptr);
	}	
}

void quadTree::searchInRadius(const int x, const int y, const int r, int & ctr, string & inCircle, string & visited)
{
	//public search
	searchInRadius(x,y,r,ctr,inCircle,visited,root);
}

void quadTree::searchInRadius(const int x, const int y,const int r,int &ctr, string & inCircle, string & visited, treeNode * &t) const
{
	// supposed to look at the circle, 13 different possibilities.
	if(t != NULL)
	{
		int low_x = x-r;
		int high_x = x+r;
		int low_y = y-r;
		int high_y = y+r;
		int xDiff = x - t->XCOORD;
		int yDiff = y - t->YCOORD;



		if((xDiff * xDiff) + (yDiff * yDiff) <= r * r)
		{
			// ý = 13 searches for all
			inCircle += t->name + ", ";
			visited += t->name + ", ";
			ctr += 1;
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SEptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SWptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NEptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NWptr);
		}

		else if(low_x >= t->XCOORD && high_y <= t->YCOORD)
		{
			//ý =1
			visited += t->name + ", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SEptr);
			
		}
		else if(high_y <= t->YCOORD && low_x <= t->XCOORD && high_x >t->XCOORD)
		{
			// ý  =2
			visited += t->name + ", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SEptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SWptr);
		}
		else if(high_x <= t->XCOORD && high_y <= t->YCOORD)
		{
			//ý = 3
			visited += t->name + ", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SWptr);
		}
		else if(low_x > t->XCOORD && low_y <= t->YCOORD && high_y > t->YCOORD)
		{
			// ý = 4
			visited += t->name + ", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SEptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NEptr);
		}
		else if(high_x <= t->XCOORD && low_y <= t->YCOORD && high_y > t->YCOORD)
		{
			// ý = 5
			visited += t->name + ", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SWptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NWptr);
		}
		else if(low_x > t->XCOORD && low_y > t->YCOORD)
		{
			//ý = 6
			visited += t->name + ", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NEptr);
		}
		else if(low_x <= t->XCOORD && high_x >= t->XCOORD && low_y > t->YCOORD)
		{
			// ý = 7
			visited += t->name + ", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NEptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NWptr);
		}
		else if (high_x <= t->XCOORD && low_y > t->YCOORD)
		{
			// ý = 8
			visited += t->name + ", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NWptr);
			
		}
		
		else if(t->XCOORD == high_x && t->YCOORD > high_y)
		{
			// corner case O.
			cout << t->name <<", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SWptr);
		}
		else if (low_x == t->XCOORD && t->YCOORD > high_y)
		{
			// corner case n
			cout << t->name <<", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SEptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SWptr);
		}
		else if (t->XCOORD > high_x && t->YCOORD ==high_y)
		{
			// corner case p 
			cout << t->name <<", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SWptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NWptr);
		}
		else if (t->XCOORD > high_x && t->YCOORD == low_y)
		{
			//corner case r
			cout << t->name <<", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NWptr);
		}
		else if(t->XCOORD < low_x && t->YCOORD == high_y)
		{
			//corner case m
			cout << t->name <<", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SEptr);
		}
		else if(t->XCOORD < low_x && t->YCOORD == low_y)
		{
			//corner case l
			cout << t->name <<", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NEptr);
		}
		else if (t->YCOORD < low_y && t->XCOORD == low_x)
		{
			//corner case k
			cout << t->name <<", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NEptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NWptr);
		}
		else if(low_x == t->XCOORD && high_y == t->YCOORD)
		{
			//corner case x
			cout << t->name <<", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SEptr);
		}
		else if(high_x == t->XCOORD && high_y == t->YCOORD)
		{
			//corner case y
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SEptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SWptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NWptr);
		}
		else if(low_x == t->XCOORD && low_y == t->YCOORD)
		{
			// corner case w
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NEptr);
		}
		else if (high_x == t->XCOORD && low_y == t->YCOORD)
		{
			//corner case 
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NWptr);
		}
		else if (t->XCOORD == high_x && t->YCOORD < low_y)
		{
			//corner case j
			cout << t->name <<", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NWptr);
		}
		
		else if (low_x <= t->XCOORD && x > t->XCOORD && y < t->YCOORD && high_y > t->YCOORD)
		{
			//ý = 9
			visited += t->name + ", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SEptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SWptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NEptr);
		}
		else if (x < t->XCOORD && t->XCOORD < high_x && y < t->YCOORD && high_y > t->YCOORD)
		{
			// ý = 10
			visited += t->name + ", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SEptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SWptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NWptr);
		}
		else if (low_x <= t->XCOORD && x > t->XCOORD && y > t->YCOORD && low_y <= t->YCOORD)
		{
			// ý = 11
			visited += t->name + ", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SEptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NEptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NWptr);
		}
		else if (x < t->XCOORD && t->XCOORD < high_x && y>t->YCOORD && t->YCOORD >= low_y)
		{
			// ý = 12
			visited += t->name + ", ";
			searchInRadius(x,y,r,ctr,inCircle,visited,t->SWptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NEptr);
			searchInRadius(x,y,r,ctr,inCircle,visited,t->NWptr);
		}
	}
}
