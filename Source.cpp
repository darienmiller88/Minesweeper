#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <random>
#include <vector>
#include <string>
using namespace std;

struct Point {
	Point(int x, int y) : x(x), y(y), symbol('b'){
	}
	int x; 
	int y;
	char symbol;
};

const int WIDTH = 10;
const int HEIGHT = 10;
const int HOWFARIN = 20;
const int HOWFARDOWN = 2;
const int COLUMNOFFSET = 4;
const int NUMBOMBS = 15;

void gotoxy(int x, int y) {
	COORD coord;

	coord.X = x;
	coord.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawMap(vector<int> &xPosOfColumns) {
	int offset = 0;

	for (size_t i = 0; i < WIDTH; i++) {
		gotoxy(HOWFARIN + offset, HOWFARDOWN - 1);
		xPosOfColumns.push_back(HOWFARIN + offset);
		cout << i + 1;
		offset += COLUMNOFFSET;
	}

	for (size_t k = 0; k < HEIGHT; k++) {
		for (size_t i = 0; i < WIDTH; i++) {
			gotoxy(xPosOfColumns[i], HOWFARDOWN + k + 1);
			cout << "x";
		}
		cout << "\t" << k + 1;
	}
}

int random(int min, int max) {
	random_device seed;
	default_random_engine rnGen(seed());
	uniform_int_distribution<int> dist(min, max);

	return dist(rnGen);
}

void setBombs(const vector<int> &xPosOfColumns, vector<Point> &bombs) {
	for (size_t i = 0; i < NUMBOMBS; i++) {
		int x = random(0, xPosOfColumns.size() - 1);
		int y = random(HOWFARDOWN + 1, HOWFARDOWN + HEIGHT);

		bombs.push_back(Point(xPosOfColumns[x], y));
	}
}

void placeBombs(const vector<Point> &bombs) {
	for (auto &bomb : bombs) {
		gotoxy(bomb.x, bomb.y);
		cout << bomb.symbol;
	}
}

int findNumBombs(int columnNumber, int rowNumber, const vector<Point> &bombs, const vector<int> &xPosOfColumns) {
	int numBombs = 0;//number of Points surrounding the point the user chose
	int row = rowNumber + HOWFARDOWN, column = xPosOfColumns[columnNumber - 1];

	for (auto &bomb : bombs) {
		//in order to check to see how many Points are surrounding the point the user chose, we will check the distance
		//between the Point and the point. This distance will tell us whether or not a Point is adjacent to the point or
		//at a corner
		if ((bomb.y == row and abs(bomb.x - column) == COLUMNOFFSET) or (bomb.x == column and abs(bomb.y - row) == 1) or
			(abs(bomb.y - row) == 1 and abs(bomb.x - column) == COLUMNOFFSET)) 
			numBombs++;
			
	}

	return numBombs;
}

bool isBomb(int columnNumber, int rowNumber, const vector<Point> &bombs, const vector<int> &xPosOfColumns) {
	int row = rowNumber + HOWFARDOWN, column = xPosOfColumns[columnNumber - 1];

	for (auto &bomb : bombs)
		if (bomb.x == column and bomb.y == row)
			return true;
	return false;

}

bool selection(const vector<int> &xPosOfColumns, const vector<Point> &bombs) {
	int row, column;

	auto checkUserChoice = [&](int &choice, const string &descriptor,  int limit) {
		while (choice <= 0 or choice > limit) {
			cout << "Select " << descriptor << ": ";
			cin >> choice;
		}
	};

	gotoxy(0, 20);
	cout << "\nSelect row: ";
	cin >> row;

	checkUserChoice(row, "row", HEIGHT);

	cout << "Select column: ";
	cin >> column;

	checkUserChoice(column, "column", WIDTH);

	if (isBomb(column, row, bombs, xPosOfColumns)) {
		gotoxy(xPosOfColumns[column - 1], row + HOWFARDOWN);
		cout << "b";
		gotoxy(0, 22);
		return false;
	}
	else {
		gotoxy(xPosOfColumns[column - 1], row + HOWFARDOWN);
		cout << findNumBombs(column, row, bombs, xPosOfColumns);
		return true;
	}
	
}

int main() {
	vector<Point> bombs;
	vector<int> xPosOfColumns;
	bool gameIsRunning = true;

	drawMap(xPosOfColumns);
	setBombs(xPosOfColumns, bombs);
	selection(xPosOfColumns, bombs);
	while (gameIsRunning) {
		gameIsRunning = selection(xPosOfColumns, bombs);
	}
	
	cout << "Oh no! you got a bomb\n";
	
}