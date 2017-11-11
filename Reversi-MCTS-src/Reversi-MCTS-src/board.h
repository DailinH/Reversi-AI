#pragma once
#include <iomanip>
#include <cmath>
#include <set>
#include <iostream>
#include "position.h"
using namespace std;

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
		curPlayer = B.curPlayer;
	}

	//copy another board
	void copy(board B) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				reversiBoard[i][j] = B.reversiBoard[i][j];
		curPlayer = B.curPlayer;
	}

	//print boards to console
	void outputBoard() {
		cout << "[O goes first]\n";
		cout << "Current Board Status:\n    ";
		for (int i = 0; i < 8; i++) {
			cout << setw(4) << i;
		}
		cout << endl;
		for (int i = 0; i < 8; i++) {
			cout << i << " : ";
			for (int j = 0; j < 8; j++) {
				if (reversiBoard[i][j] == -1)
					cout << setw(4) << "";
				else
				{
					if(reversiBoard[i][j] == 0)
						cout << setw(4) << 'O';
					else 
						cout << setw(4) << 'X';
				}
				//	cout << setw(4) << reversiBoard[i][j];
			}
			cout << endl;
		}
		cout << endl;
	}

	//place a piece on position pos if pos is valid
	////pos: the position where a piece is to be placed
	bool placeOn(position pos, int &player) {
		if (!isValidStep(pos)) return false;
		//if (reversiBoard[pos.x][pos.y] != -1) return false;
		reversiBoard[pos.x][pos.y] = player;
		flip(pos);
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
	
	//get current game status for white pieces;
	//2: win;
	//1: lose;
	//0: draw;
	//-1: game is going on	
	int getStatus(set<position> &tmp) {	
		if (!getAllValidMoves(tmp)) {
			curPlayer = !curPlayer;
			if (!getAllValidMoves(tmp)) {
				//int opponent = abs(curPlayer - 1);
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
		}
		return -1;
	}

	//privided that position p is a valid position, we use flip to flip all the pieces available.
	void flip(position p) {
		int direction = -1;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (i == 0 && j == 0) continue;
				direction++;
				//check all directions to see if it is valid;
				//1. a piece of the opponents' beside it
				//2. on that direction, there is a piece of my own side;
				if (p.dir[direction] != 0) {
					int x = p.x, y = p.y;
					reversiBoard[x][y] = curPlayer;
					int cnt = 0;
					bool cont = true;// continue if still valid 
					while (++cnt && cont) {
						int x1 = x + cnt*i, y1 = y + cnt*j;//(x1, y1)stands for checking same-side pieces
						if (x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7) break;
						//check if this is a piece of the same side
						if (reversiBoard[x1][y1] == curPlayer) 	break;
						reversiBoard[x1][y1] = curPlayer;
					}
				}
			}
		}

	}
	
//private:
	//store all the current valid choices
	//may not be of use in this class. perhaps it should be shifted to mcts
	//set<position> curValid;

	//player: 1 or 0, depending on the turn of the player
	int curPlayer;
	//store the board status
	int reversiBoard[8][8];
};
