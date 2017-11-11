#include <iostream>
#include <cmath>
#include <set>
#include <iomanip>
#include <ctime>
#include "board.h"
#include "position.h"
const clock_t simulationTime = 2 * CLOCKS_PER_SEC;

class MonteCarlo
{
public:
	MonteCarlo() {};


	//return the result if the game has ended
	void getResult(board &B, int state) {
		cout << "the game is over!\n";
		if (state == 0)
			cout << "The score was tied~";
		else if (state == 1) {
			if (B.curPlayer == 0)
				cout << "You won!";
			else cout << "You Lost!";
		}
		else {
			if (B.curPlayer == 1)
				cout << "You won!";
			else cout << "You Lost!";
		}
	}

	position getInputFromIostream() {
		cout << "it's your turn! ";
		int x, y;
		cout << "Please input your selection:\n";
		cin >> x >> y;
		position pos(x, y);
		return pos;
	}

	//run simulation process
	void runSimulation(board &B, int &win) {
		//if the game is going on
		set<position> currentNodes;
		int curState = B.getStatus(currentNodes);
		if (curState == -1) {
			board tmp(B);
			//randomly choose a valid node to proceed
			int sz = currentNodes.size();
			srand((unsigned int)(time(NULL)));
			int seq = rand() % sz;
			int n = 0;
			for (set<position>::iterator iter = currentNodes.begin(); iter != currentNodes.end(); iter++) {
				if (n == seq) {
					tmp.placeOn(*iter, tmp.curPlayer);
					//tmp.outputBoard();
					//	system("pause");
					return runSimulation(tmp, win);
				}
				n++;
			}
		}
		//if the game has ended
		else {
			//cout << "simulation completed." << endl;
			//system("pause");
			if (curState == 2) {
				win = 1;
			}
			else {
				win = 0;
			}
			return;
		}

	}

	//traditional MonteCarlo method: choose the node with the greatest number of wins
	void traditionalSelect(board &B, int aiPlayer) {
		set<position> validp;
		board C(B);
		int state = C.getStatus(validp);
		if (state != -1) {
			getResult(B, state);
			return;
		}
		int tmpPlayer = B.curPlayer;
		if (aiPlayer != B.curPlayer) {
			position pos = getInputFromIostream();
			if (B.isValidStep(pos)) {
				B.placeOn(pos, B.curPlayer);
				system("cls");
				B.outputBoard();
				cout << "hmmm...lemme think for a moment\n";
				traditionalSelect(B, aiPlayer);
			}
			else {
				cout << "I hope you surely know what you are doing, are you sure that's a valid step? XD" << endl;
				cout << "Try again![t]\n";
				char tryAgain;
				cin >> tryAgain;
				system("cls");
				B.outputBoard();
				traditionalSelect(B, aiPlayer);
			}

		}
		else {
			int moves[64] = { 0 };
			int sz = validp.size();
			int cnt = 0;
			clock_t beginTime = (clock_t)time(NULL);
			int c = 0;
			int curTime = (clock_t)time(NULL);
			while (curTime < beginTime + simulationTime / 1000) {
				for (set<position>::iterator iter = validp.begin(); iter != validp.end(); iter++) {
					tmpPlayer = B.curPlayer;
					int save = cnt%sz;
					cnt++;
					board tmp(B);
					tmp.placeOn(*iter, tmp.curPlayer);
					int res = -1;
					runSimulation(tmp, res);
					//cout << "ai will " << res << endl;
					moves[save] += res;
				}
				curTime = (clock_t)time(NULL);
			}
			int greatest = 0;
			position greatestPos;
			int i = 0;
			for (set<position>::iterator iter = validp.begin(); iter != validp.end(); iter++) {
				if (moves[i++] > greatest) {
					greatestPos = *iter;
					greatest = moves[i - 1];
				}
			}
			B.placeOn(greatestPos, B.curPlayer);
			cout << "go on!";
			system("cls");
			B.outputBoard();
			traditionalSelect(B, aiPlayer);
			//choose the greatest number of wins from all possible moves;
		}
	}

	//UCB1 MonteCarlo method: choose the node by constructing statistical confidence intervals \lineover{x_i} \pm \sqrt{\frac{2lnn}{n}}
	void UCB1Select(board&B, int aiPlayer) {
		set<position> validp;
		board C(B);
		int state = C.getStatus(validp);
		if (state != -1) {
			getResult(B, state);
			return;
		}
		int tmpPlayer = B.curPlayer;
		if (aiPlayer != B.curPlayer) {
			position pos = getInputFromIostream();
			if (B.isValidStep(pos)) {
				B.placeOn(pos, B.curPlayer);
				system("cls");
				B.outputBoard();
				cout << "hmmm...lemme think for a moment\n";
				UCB1Select(B, aiPlayer);
			}
			else {
				cout << "I hope you surely know what you are doing, are you sure that's a valid step? XD" << endl;
				cout << "Try again![t]\n";
				char tryAgain;
				cin >> tryAgain;
				system("cls");
				B.outputBoard();
				UCB1Select(B, aiPlayer);
			}

		}
		else {
			//the total number of winnings
			int moves[64] = { 0 };
			//the total number of simulations for the node
			int N[64] = { 0 };
			double statisticsConfidenceIntervals[64] = { 0 };
			int sz = validp.size();
			// the total number of simulations
			int cnt = 0;
			//the total number of plays
			clock_t beginTime = (clock_t)time(NULL);
			int curTime = (clock_t)time(NULL);
			for (set<position>::iterator iter = validp.begin(); iter != validp.end(); iter++) {
				tmpPlayer = B.curPlayer;
				int save = cnt%sz;
				cnt++;
				board tmp(B);
				tmp.placeOn(*iter, tmp.curPlayer);
				int res = -1;
				runSimulation(tmp, res);
				moves[save] += res;
				N[save]++;
			}
			//for (int i = 0; i < sz; i++) {
			//	statisticsConfidenceIntervals[i] = (double)(moves[i]) / cnt + sqrt(log(N[i]) / cnt);
			//}
			while (curTime < beginTime + simulationTime / 1000) {
				int iteration = 0;
				for (set<position>::iterator iter = validp.begin(); iter != validp.end(); iter++) {
					iteration++;
					int largest = 0;
					position largestpos = 0;
					int i = 0;
					for (set<position>::iterator iterin = validp.begin(); iterin != validp.end(); iterin++) {
						i++;
						statisticsConfidenceIntervals[i] = (double)(moves[i]) / cnt + sqrt(log(N[i]) / cnt);
						if (statisticsConfidenceIntervals[i] > largest) {
							largestpos = *iterin;
							largest = statisticsConfidenceIntervals[i];
						}
					}
					N[largest]++;
					int save = cnt%sz;//the sequence num of the machine
					cnt++;

					tmpPlayer = B.curPlayer;
					board tmp(B);
					tmp.placeOn(largestpos, tmp.curPlayer);
					int res = -1;
					runSimulation(tmp, res);
					moves[largest] += res;
					statisticsConfidenceIntervals[largest] = (double)(moves[largest]) / cnt + sqrt(log(N[largest])/cnt);

				}
				curTime = (clock_t)time(NULL);
			}
			int greatest = 0;
			position greatestPos;
			//choose the greatest statistics confidence intervals from all
			int i = 0;
			for (set<position>::iterator iter = validp.begin(); iter != validp.end(); iter++) {
				if (statisticsConfidenceIntervals[i++] > greatest) {
					greatestPos = *iter;
					greatest = moves[i - 1];
				}
			}
			//cout << statisticsConfidenceIntervals[greatest];
			B.placeOn(greatestPos, B.curPlayer);
			cout << "go on!";
			system("cls");
			B.outputBoard();
			UCB1Select(B, aiPlayer);
			//choose the greatest number of wins from all possible moves;
		}
	}
};
