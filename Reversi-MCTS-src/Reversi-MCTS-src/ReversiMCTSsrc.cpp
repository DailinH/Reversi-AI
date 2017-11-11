// ReversiMCTSsrc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <set>
#include <iomanip>
#include <ctime>
#include "board.h"
#include "position.h"
#include "MonteCarlo.h"
using namespace std;


int main()
{
	cout << "Welcome to the Reversi-AI! \n";
	cout << "Which mode do you want to take?[Traditional MCTS mode(t)/UCB1 mode(u)]  ";
	char mode;
	cin >> mode;
	if (mode == 't') {
		cout << "Do you want to go first ? [y/n]  ";
		char decision;
		while (cin >> decision) {
			if (decision == 'y') {
				board Board(0);
				Board.outputBoard();
				MonteCarlo a;
				a.traditionalSelect(Board, 1);
				//a.UCB1Select(Board, 1);
			}
			else if (decision == 'n') {
				board Board(0);
				Board.outputBoard();
				MonteCarlo a;
				a.traditionalSelect(Board, 0);
				//	a.UCB1Select(Board, 0);

			}
			else {
				cout << "emmm....sorry, what did you say again?";
			}
		}
	}
	else if(mode == 'u') {
		cout << "To tell you the truth, this mode is bug-full ©c(*¡£>§¥<)o©b\n";
		cout << "Do you want to go first ? [y/n]  ";
		char decision;
		while (cin >> decision) {
			if (decision == 'y') {
				board Board(0);
				Board.outputBoard();
				MonteCarlo a;
				//a.traditionalSelect(Board, 1);
				a.UCB1Select(Board, 1);
			}
			else if (decision == 'n') {
				board Board(0);
				Board.outputBoard();
				MonteCarlo a;
				//a.traditionalSelect(Board, 0);
				a.UCB1Select(Board, 0);

			}
			else {
				cout << "emmm....sorry, what did you say again?";
			}
		}
	}
	else {
		cout << "emmm....sorry, what did you say again?";
	}

    return 0;
}

