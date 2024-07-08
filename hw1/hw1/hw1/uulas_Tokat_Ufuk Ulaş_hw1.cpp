
#include <iostream>
#include <string>
#include <vector>
#include "Stack.cpp"
#include <fstream>
#include "randgen.h"

using namespace std;


struct Cell  // creating a struct to store all necessary information about a cell of the maze
{
	int lrub[4];  // a matrix to store walls Left-rigth-up-bottom 0 = no wall, 1 = wall
	bool visited; // shows that the cell is visited or not false = not visited, true = visited
	int x;   // column number
	int y;  // row number

	Cell(){  // a default contructer which will be called while stack is formed
		x = NULL;
		y = NULL;
		visited = false;
		for(int i=0;i<4;i++){
			lrub[i]=NULL;
		}
	}
	Cell(int a, int b){ // a parameterized constructer
		x = b; //sets column number to b
		y = a; //sets row number to a
		for(int i=0;i<4;i++){ //sets all walls to 1
			lrub[i]=1;
		}
		
		visited = false;
	}

};


// a function to print stack reversly in path finding part
void reversePrint(Stack<Cell> &print, ofstream & file){ 
	Cell temp;
	string out;
	if(!print.isEmpty()){
		print.pop(temp);
		reversePrint(print, file); // recersively calls itself
		file << temp.x << " " << temp.y<<endl; // prints to file
	}
	else{
		return;
	}
	
	
	
}

int main(){

	// defining all necessary inputs at once
	int maze_number, rows, columns, ID, entry_x, entry_y, exit_x, exit_y;

	// asking how many maze will be created
	cout<<"Enter the number of mazes: ";
	cin>>maze_number;
	cout<<endl;

	// asking the size of the mazes
	cout<<"Enter the number of rows and columns (M and N): ";
	cin>>columns>>rows;
	cout<<endl;

	//defining a vector vector vector to store all mazes
	vector<vector<vector<Cell>>> mazeList;

	//creating all mazes
	for(int i=1; i<=maze_number;i++){
		vector<vector<Cell>> tempMaze;
		for(int j=0; j<rows; j++){
			vector<Cell> tempRow;
			for(int k=0; k<columns; k++){
				Cell tempCell = Cell(j,k);
				tempRow.push_back(tempCell); //but every cell has 4 wall rigth now
			}
			tempMaze.push_back(tempRow);
		}
		
		//creating a stack to follow the maze
		Stack<Cell> tempStack;
		tempStack.push(tempMaze[0][0]); //pushing the first cell

		tempMaze[0][0].visited = true; //making first cell visited
		Cell currentCell = tempMaze[0][0]; //making first cell of the maze starting point

		while(!tempStack.isEmpty()){// works until the stack is empty

			bool check = false; // a variable to follow when to pass the next cell

			int range = 3; //defines the range random number

			int availability = 0; //checks the avaliability of the the directions
			                      //if all 4 direction is not avaliable then this parameter will start going back

			vector<int> rangeVec; //stores possible ways
			for(int i=0; i<4; i++){ // if a way has been tried and not possible to go 
				rangeVec.push_back(i); //it will be removed from this vector
			}

			while(check==false){ //works until the a way is possible to go or all the ways are impossible
				RandGen r;	
				int destination = rangeVec[r.RandInt(range)]; //creates a random number and choose possible way in that index in rangevec
				//int destination = r.RandInt(range);
				if (destination==0){ //destination is left
					if(currentCell.x-1 >= 0 && tempMaze[currentCell.y][currentCell.x-1].visited==false){//checks that if the cell to go is in range and not visited
						tempMaze[currentCell.y][currentCell.x].lrub[0] = 0; //sets left wall of the current cell to 0
						tempMaze[currentCell.y][currentCell.x-1].visited = true; //sets the cell to go as visited
						tempMaze[currentCell.y][currentCell.x-1].lrub[1] = 0; //sets rigth wall of the next cell to 0
						tempStack.push(currentCell);//push the cuurent cell
						currentCell = tempMaze[currentCell.y][currentCell.x-1];//make the next cell current to move on
						check = true; //ends the loop
					}
					//left cell is not possible
					else{
						range = range-1;
						rangeVec.erase(find(rangeVec.begin(),rangeVec.end(),destination));	
						availability++;
					}
				}
				else if (destination==1){ //destination is rigth
					if(currentCell.x+1 < columns && tempMaze[currentCell.y][currentCell.x+1].visited==false){//checks that if the cell to go is in range and not visited
						tempMaze[currentCell.y][currentCell.x].lrub[1] = 0;//sets rigth wall of the current cell to 0
						tempMaze[currentCell.y][currentCell.x+1].visited = true;//sets the cell to go as visited
						tempMaze[currentCell.y][currentCell.x+1].lrub[0] = 0;//sets left wall of the next cell to 0
						tempStack.push(currentCell); //push the cuurent cell
						currentCell = tempMaze[currentCell.y][currentCell.x+1]; //make the next cell current to move on
						check = true; //ends the loop
					}
					//rigth cell is not possible
					else{
						range = range-1;
						rangeVec.erase(find(rangeVec.begin(),rangeVec.end(),destination));	
						availability++;
					}
				}
				else if (destination==2){ //destination is up
					if(currentCell.y+1 < rows && tempMaze[currentCell.y+1][currentCell.x].visited==false){//checks that if the cell to go is in range and not visited
						tempMaze[currentCell.y][currentCell.x].lrub[2] = 0;//sets upper wall of the current cell to 0
						tempMaze[currentCell.y+1][currentCell.x].visited = true;//sets the cell to go as visited
						tempMaze[currentCell.y+1][currentCell.x].lrub[3] = 0;//sets bottom wall of the next cell to 0
						tempStack.push(currentCell);//push the cuurent cell
						currentCell = tempMaze[currentCell.y+1][currentCell.x];//make the next cell current to move on
						check = true; //ends the loop
					}
					//upper cell is not possible
					else{
						range = range-1;
						rangeVec.erase(find(rangeVec.begin(),rangeVec.end(),destination));	
						availability++;
					}
				}
				else if (destination==3){ //destination is bottom
					if(currentCell.y-1 >= 0 && tempMaze[currentCell.y-1][currentCell.x].visited==false){//checks that if the cell to go is in range and not visited
						tempMaze[currentCell.y][currentCell.x].lrub[3] = 0;//sets bottom wall of the current cell to 0
						tempMaze[currentCell.y-1][currentCell.x].visited = true;//sets the cell to go as visited
						tempMaze[currentCell.y-1][currentCell.x].lrub[2] = 0;//sets upper wall of the next cell to 0
						tempStack.push(currentCell);//push the cuurent cell
						currentCell = tempMaze[currentCell.y-1][currentCell.x];//make the next cell current to move on
						check = true; //ends the loop
					}
					//lower cell is not possible
					else{
						range = range-1;
						rangeVec.erase(find(rangeVec.begin(),rangeVec.end(),destination));	
						availability++;
					}
				}
				if(availability==4){ //all 4 ways is not possible from current cell
					check=true;
				}
			}
			if(availability==4){ //goo back to the cell before
				tempStack.pop(currentCell);
			}
		}

		tempStack.~Stack(); //destroy stack to avoid memory leak


		//printing the maze
		mazeList.push_back(tempMaze);
		ofstream outFile;
		string fileName = "maze_" + to_string(i) + ".txt";
		outFile.open(fileName.c_str());
		outFile << rows << " " << columns << endl;

		for(int j=0; j<rows; j++){
			for(int k=0; k<columns; k++){
				outFile << "x=" << tempMaze[j][k].x  << " y=" << tempMaze[j][k].y << " l=" << tempMaze[j][k].lrub[0] << " r=" << tempMaze[j][k].lrub[1] << " u=" << tempMaze[j][k].lrub[2] << " d=" << tempMaze[j][k].lrub[3] << endl;
			}
		}		
	}


	// getting inputs to find path
	
	
	cout<<"All mazes are generated.";
	cout<<endl;
	cout<<endl;
	
	//which maze
	cout<<"Enter a maze ID between 1 to 5 inclusive to find a path: ";
	cin>>ID;
	cout<<endl;
	//where to begin
	cout<<"Enter x and y coordinates of the entry points (x,y) or (column,row): ";
	cin>>entry_x >>entry_y;
	cout<<endl;
	//where to go
	cout<<"Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	cin>>exit_x >>exit_y;
	cout<<endl;
	
	//seperating the choosen maze
	vector<vector<Cell>> pathMaze = mazeList[ID-1];

	//making its cells unvisited
	for(int j=0; j<rows; j++){
		for(int k=0; k<columns; k++){
			pathMaze[j][k].visited = false;
		}
	}

	//defining a stack to follow path
	Stack<Cell> pathStack;
	pathMaze[entry_y][entry_x].visited = true;
	Cell currentPath = pathMaze[entry_y][entry_x];

	//works until the current cell is the destination
	while((currentPath.x == exit_x && currentPath.y == exit_y)==false){
		bool flag = true;
		while(flag){
			if(currentPath.lrub[0]==0 && pathMaze[currentPath.y][currentPath.x-1].visited == false){//trying the left cell
				pathStack.push(currentPath);//pushes the current cell 
				pathMaze[currentPath.y][currentPath.x-1].visited = true; //makes next cell visited
				currentPath = pathMaze[currentPath.y][currentPath.x-1]; //makes cuurent cell next cell
				flag = false;
			}
			else if(currentPath.lrub[1]==0 && pathMaze[currentPath.y][currentPath.x+1].visited == false){//trying the rigth cell
				pathStack.push(currentPath);
				pathMaze[currentPath.y][currentPath.x+1].visited = true;
				currentPath = pathMaze[currentPath.y][currentPath.x+1];
				flag = false;
			}
			else if(currentPath.lrub[2]==0 && pathMaze[currentPath.y+1][currentPath.x].visited == false){//trying the upper cell
				pathStack.push(currentPath);
				pathMaze[currentPath.y+1][currentPath.x].visited = true;
				currentPath = pathMaze[currentPath.y+1][currentPath.x];
				flag = false;
			}
			else if(currentPath.lrub[3]==0 && pathMaze[currentPath.y-1][currentPath.x].visited == false){//trying the lower cell
				pathStack.push(currentPath);
				pathMaze[currentPath.y-1][currentPath.x].visited = true;
				currentPath = pathMaze[currentPath.y-1][currentPath.x];
				flag = false;
			}
			else{
				//pathStack.pop(currentPath); //go back 
				pathStack.pop(currentPath); //set current cell
				//pathStack.push(currentPath); //push it back
			}
		}
	}

	pathStack.push(pathMaze[exit_y][exit_x]);//pushing the last cell

	//printing the path
	ofstream out;
	string fileName = "maze_" + to_string(ID) + "_" + to_string(entry_x) + "_" + to_string(entry_y) + "_" + to_string(exit_x) + "_" + to_string(exit_y) +".txt";
	out.open(fileName.c_str());
	reversePrint(pathStack,out);
	
	pathStack.~Stack();//destroying the Stack to avoid memory leak
	return 0;
}