#include <iostream>
#include <fstream>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
using namespace std;

class Point { //stack to track traveled and visited coordinates
public:
	int x, y; //coords
	char path[30][100]; //traveled array
	char visit[30][100]; //visited array

	Point() { x = 0; y = 1; }
	void push(int x, int y) { path[y][x] = '1'; }
	void pop(int direction) {
		if (direction == 1) { //backtrack path left
			path[y][x - 1] = '0'; //clear current location
			push(x + 1, y); //block dead end in traversed array
		}
		else if (direction == 2) { //backtrack path up
			path[y - 1][x] = '0'; //clear current location
			push(x, y + 1); //block dead end in traversed array
		}
		else if (direction == 3) { //backtrack path right
			path[y][x + 1] = '0'; //clear current location
			push(x - 1, y); //block dead end in traversed array
		}
		else if (direction == 4) { //backtrack path down
			path[y + 1][x] = '0'; //clear current location
			push(x, y - 1); //block dead end in traversed array
		}
		visit[y][x] = '1'; //push flag to visited array
	}
};

class Maze {
public:
	ifstream maze;
	char mArr[30][100];
	int direction; //mouse's last movement
	Point p;

	void pushMaze(); //read maze from file
	void drawMaze(); //display maze from 2d array
	bool solve(); //start solving of maze process
	void gotoxy(int x, int y); //go to function (displays 'mouse' movement)
	void push();
	void pop(int dir);
	void colorTxtB(); //set output to blue
	void colorTxtR(); //set output to red
	void colorTxtG(); //set output to green
};
void Maze::colorTxtR() {
	//red colored text!
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute
	(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
}
void Maze::colorTxtB() {
	//blue colored text!
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute
	(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
void Maze::colorTxtG() {
	//green colored text!
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute
	(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}
void Maze::pushMaze() {
	maze.open("Maze.txt"); //open maze file
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 100; j++)
			maze >> mArr[i][j]; //initialize array based on file

	maze.close();
}
void Maze::drawMaze() {
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			colorTxtB(); //set output to blue
			if (mArr[i][j] == '1') //display wall
				cout << char(219);
			if (mArr[i][j] == '0') //display empty area
				cout << " ";
			if (mArr[i][j] == '2') {//display bomb char 
				colorTxtR(); //set bomb color to red
				cout << "B";
			}
			if (mArr[i][j] == '3') { //display starting point
				colorTxtG();
				cout << "S";
			}
		}
		colorTxtB(); //reset output to blue
		cout << endl;
	}
}
bool Maze::solve() {
	gotoxy(p.x, p.y);

	while (p.x != 99 && p.y != 29) { //check if mouse is at end of maze
									 //move right
		if (mArr[p.y][p.x + 1] != '1' && p.x + 1 != 100 && p.path[p.y][p.x + 1] != '1' && p.visit[p.y][p.x + 1] != '1') {
			p.x++; //increment x
			direction = 1; //set direction
			push();

		}//move up
		else if (mArr[p.y - 1][p.x] != '1' && p.y - 1 != 0 && p.path[p.y - 1][p.x] != '1' && p.visit[p.y - 1][p.x] != '1' && direction != 2) {
			p.y--; //decrement y
			direction = 4;
			push();

		} //move down
		else if (mArr[p.y + 1][p.x] != '1' && p.y + 1 != 30 && p.path[p.y + 1][p.x] != '1' && p.visit[p.y + 1][p.x] != '1') {
			p.y++; //increment y
			direction = 2;
			push();

		}//move left
		else if (mArr[p.y][p.x - 1] != '1' && p.x - 1 != 0 && p.path[p.y][p.x - 1] != '1' && p.visit[p.y][p.x - 1] != '1') {
			p.x--; //decrement x
			direction = 3;
			push();

		}
		else //if at dead end pop stack
			pop(direction);

	}
	return true; //if maze solved return true
}
void Maze::gotoxy(int x, int y) { //goto function

	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

void Maze::pop(int dir) {
	p.pop(dir);
	if (dir == 1 && mArr[p.y][p.x - 1] != '1') { //backtrack left if no wall exists
												 //set new mouse location
		--p.x;
	}
	else if (dir == 2 && mArr[p.y - 1][p.x] != '1') { //backtrack up if no wall exists
													  //set new mouse location
		--p.y;
	}
	else if (dir == 3 && mArr[p.y][p.x + 1] != '1') { //backtrack right if no wall exists
													  //set new mouse location
		++p.x;
	}
	else if (dir == 4 && mArr[p.y + 1][p.x] != '1') { //backtrack down if no wall exists
													  //set new mouse location
		++p.y;
	}

	//display mouse movements AKA THE CURSOR
	gotoxy(p.x, p.y);
	Sleep(50);
}

void Maze::push() {
	p.push(p.x, p.y); //track traversed point

					  //display mouse moving AKA THE CURSOR
	gotoxy(p.x, p.y);
	Sleep(50);
}

void main() {
	Maze Maze;
	Maze.pushMaze();
	Maze.drawMaze();

	Maze.solve();

	//colored output upon mouse hitting bomb at end of maze
	Maze.colorTxtR();
	system("cls");
	cout << "EXPLOSION HAS OCCURRED! OH NO!" << endl;

	system("pause");
}