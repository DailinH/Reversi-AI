// ReversiMCTSsrc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;
struct position
{
	position(int a, int b) {
		x = a;
		y = b;
		memset(dir, 0, sizeof(dir));
		//for (int i = 0; i < 8; i++)
		//	dir[i] = d[i];
	}
	bool operator<(const position &b) const {
		if (x < b.x) return true;
		if (x > b.x) return false;
		if (x == b.x) {
			if (y <= b.y) return true;
		}
	}
	int x;
	int y;
	int dir[8];//check availability on eight directions
};

class board
{
public:
	//default constructor(initialization)
	board(int P = 0) {
		//  1 stands for the white side;
		//  0 stands for the dark side;
		// -1 stands for empty grids.
		memset(reversiBoard, -1, sizeof(reversiBoard));
		reversiBoard[3][3] = 1; 
		reversiBoard[4][4] = 1;
		reversiBoard[3][4] = 0;
		reversiBoard[4][3] = 0;
		//default role: white
		curPlayer = P;
		//outputBoard();
	}

	//copy constructor
	board(board &B) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				reversiBoard[i][j] = B.reversiBoard[i][j];
	}

	//print boards to console
	void outputBoard() {
		for (int i = 0; i < 8; i++) {
			cout << i << " : ";
			for (int j = 0; j < 8; j++) {
				cout <<setw(4) <<  reversiBoard[i][j] ;
			}
			cout << endl;
		}
	}

	//place a piece on position pos if pos is valid
	////pos: the position where a piece is to be placed
	bool placeOn(position pos, int &player) {
		if (!isValidStep(pos)) return false;
		//if (reversiBoard[pos.x][pos.y] != -1) return false;
		reversiBoard[pos.x][pos.y] = player;
		player = !player;
		return true;
	}

	//judge if a step is valid
	bool isValidStep(position &pos) {
		int directions = -1;
		int x = pos.x, y = pos.y;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (i == 0 && j == 0) continue;
				directions++;
				//check all directions to see if it is valid;
				//1. a piece of the opponents' beside it
				//2. on that direction, there is a piece of my own side;
				if (reversiBoard[x + i][y + j] != abs(1 - curPlayer)) continue;
				int cnt = 1;
				bool cont = true;// continue if still valid 
				while (cnt++ && cont) {
					int x1 = x + cnt*i, y1 = y + cnt*j;//(x1, y1)stands for checking same-side pieces
					if (x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7) break;
					//check if this is a piece of the same side
					if (reversiBoard[x1][y1] == curPlayer) {
						pos.dir[directions] = 1;
						break;
					}
					if (reversiBoard[x1][y1] == -1) {
						break;
					}
				}
			}
		}
		int sum = 0;
		for (int i = 0; i < 8; i++) sum += pos.dir[i];
		if (sum) return true;
		return false;
	}

	//attain all valid choices for the current step and store them in "set<position> curValid"
	bool getAllValidMoves(set<position> &curValid) {
		//determines whether each step is valid
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (reversiBoard[i][j] == -1) {
					position p(i, j);
					if (isValidStep(p)) {
						//reversiBoard[i][j] = 2;
						curValid.insert(p);
					}
				}
			}
		}
		if (curValid.empty()) return false;  
		else return true;
	}
	
	//player: 1 or 0, depending on the turn of the player
	int curPlayer;

	//store all the current valid choices
	//may not be of use in this class. perhaps it should be shifted to mcts
	//set<position> curValid;
	
	//get current game status for white pieces;
	int getStatus() {
		//2: win;
		//1: lose;
		//0: draw;
		//-1: game is going on
		set<position> tmp;
		if (!getAllValidMoves(tmp)) {
			int opponent = abs(curPlayer - 1);
			int win = 0, lose = 0;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (reversiBoard[i][j] == 1)
						win++;
					else if (reversiBoard[i][j] == 0)
						lose++;
					else continue;
				}
			}
			if (win == lose) return 0;
			else if (win < lose) return 1;
			else return 2;
		}
		return -1;
	}

	int reversiBoard[8][8];
};

int main()
{
	board Board;
	Board.outputBoard();
	set<position> pos;
	Board.getAllValidMoves(pos);
	Board.outputBoard();

	//int x, y;
	//bool white = true;
	//while (cin >> x >> y) {
	//	position pos(x, y);
	//	Board.outputBoard();

	//}
    return 0;
}

