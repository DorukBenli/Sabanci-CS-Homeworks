#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "MPQ.h"

using namespace std;


int leftChild(int i)
{
	//this code is taken from the slide 13 sorting(2).
	return 2*i+1;
}


void percDown(vector<vector<int>> &arr, int i, int n)
{
	//this code is taken from the slide 13 sorting(2) but modified.
	//i is the index to be percolated down.
	//n is the logical size.
	//one issue is if i have [0,2], [0,5], [0,31] then [0,31] must be first.
	int child;
	vector<int> temp;
	for(temp = arr[i]; leftChild(i) < n; i = child)
	{
		child = leftChild(i);
		//compare the x values 
		if(child != n-1 && arr[child][0] < arr[child+1][0])
		{
			child++;
		}
		//compare the heights for finding the min child with max height
		else if(child != n-1 && arr[child][0] == arr[child+1][0] && arr[child][1] > arr[child+1][1])
		{
			child++;
		}

		if(arr[child][0]> temp[0])
		{
			swap(arr[i], arr[child]); //swap hole and max children
		}
		//if y value is smaller then swap hole with child.
		else if(arr[child][0] == temp[0] && arr[child][1] < temp[1])
		{
			swap(arr[i], arr[child]);
		}
		
		else{
			break;
		}
	}
	arr[i] = temp; //place temp in  its final position

}

void heapSort(vector<vector<int>> &arr)
{
	//build heap
	for(int i =arr.size()/2; i>=0; i--)
	{
		percDown(arr,i,arr.size());
	}

	//sort
	for(int j= arr.size()-1; j > 0; j--)
	{
		swap(arr[0],arr[j]);
		percDown(arr,0,j);
		
	}
}

int main()
{
	// opening the file
	string filename = "input.txt",line;
	ifstream file;
	file.open(filename.c_str());
	int ctr = 0, city_num;
	vector<vector<int>> markList;
	MPQ priority_queue;
	//line x1, y , x2
	while(getline(file,line))
	{	
		ctr += 1;
		int lbl = ctr;
		if(ctr > 1)
		{
			int x1, y, x2;
			int idx1, idx2;
			vector<int> start, end;
			idx1 = line.find(" ");
			idx2 = line.rfind(" ");
			//finding the first x
			x1 = stoi(line.substr(0,idx1)); // start of building 
			//finding the second x
			x2 = stoi(line.substr(idx2+1)); //end of building 
			y = stoi(line.substr(idx1+1, idx2)); // height of building 
			// when a building ends make the height negative so we can understand that the building has ended.
			start.push_back(x1);
			start.push_back(y);
			start.push_back(lbl);
			// for the ending values, we store the negative values
			end.push_back(x2);
			end.push_back(-y);
			end.push_back(lbl);
			markList.push_back(start);
			markList.push_back(end);
			// the starting point is picked as negative due to fact that it would print negative valyes
			//if we picked positive values for left x values.
		}
		else
		{
			city_num = stoi(line);
			priority_queue.setLocationSize(city_num);
		}
	}
	// to keep the order of consecutive buildings we sort the indexes accordingly.
	heapSort(markList);

	//priority queue is used to keep the track of which building is the tallest.

	//we need a previous values for making comparisons with current max element
	int comp = 0;

	for(int i=0; i <markList.size(); i++)
	{
		//sweep through the list.
		if(i == 0 && markList[0][0] != 0)
		{
			//this check prints 0 0 if no building stands at x = 0
			cout << 0 <<" "<< 0 << endl;
		}
		if(markList[i][1] > 0)
		{
			//start point of the building --> push to pq
			priority_queue.insert(markList[i][1], markList[i][2]);
		}
		else{
			//means we have reached an endpoint, remove form queue
			priority_queue.Remove(markList[i][2]);
		}

		//check for the previous value
		int current_val = priority_queue.getMax(); //highest y axis.

		if(comp != current_val)
		{
			//we need a previous values for making comparisons with current max element
			cout << markList[i][0] <<" " << current_val << endl;
			comp = current_val;
		}
	}
	return 0;
}