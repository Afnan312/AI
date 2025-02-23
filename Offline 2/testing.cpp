```````````````````````````````````````````mancala`````````````````````````````````````````````````````
#define CPU_WIN 10000 // high value
#define HUMAN_WIN -10000 // low value
#define DRAW -5000 // low value: CPU doesn't want to play draw
#define NO_MOVES -100000
#define MAX_DEPTH 4 // number of moves to lookahead by CPU (when tactic = lookahead)

#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>
#include <climits>
#include "mancala.h"

using namespace std;

// constructor
MancalaBoard::MancalaBoard(int pits, int stones) : pits(pits), stones(stones) {
	if (pits <= 0) cout << "Number of pits must be > 0" << endl;
	if (stones <= 0) cout << "Number of stones must be > 0" << endl;
	for (int i=0; i<=(2*pits+1); i++) {
		field.push_back(stones);
	}
	// reset stores to 0
	field[pits] = 0;
	field[2*pits+1] = 0;
}

// Function that prints the board
void MancalaBoard::printBoard(void) const {
	//Default board
	//CPU: 12|11|10|9 |8 |7 |
	// -------------------------
	// |  |4 |4 |4 |4 |4 |4 |  |
	// |0 |--+--+--+--+--+--|0 |
	// |  |4 |4 |4 |4 |4 |4 |  |
	// -------------------------
	//HUM: 0 |1 |2 |3 |4 |5 |

	//line 1
	cout << "CPU: ";
	for(int i=(2*pits); i>pits; i--) {
		if (i < 10) {
			cout <<i<<" |";
		} else {
			cout <<i<<"|";
		}
	}
	cout<<endl;
	//line 2
	cout << " ----";
	for(int i=pits; i>0; i--) {
		cout << "---";
	}
	cout << "---" <<  endl;
	//line 3
	cout << " |  |";
	for(int i=(2*pits); i>pits; i--) {
		cout << field[i];
		if (field[i]<10) {
			cout << " |";
		} else {
			cout << "|";
		}
	}
	cout << "  |" << endl;
	//line 4
	cout << " |" << field[(2*pits+1)];
	if (field[2*pits+1] < 10) {
		cout << " |";
	} else {
		cout << "|";
	}
	for(int i=pits; i>1; i--) {
		cout << "--+";
	}
	cout << "--|" << field[pits];
	if (field[pits]<10) {
		cout << " |";
	} else {
		cout << "|";
	}
	cout << endl;
	//line 5
	cout << " |  |";for(int i=0; i<pits; i++) {
		cout << field[i];
		if (field[i]<10) {
			cout << " |";
		} else{
			cout << "|";
		}
	}
	cout << "  |" << endl;
	//line 6
	cout << " ----";
	for(int i=pits; i>0; i--) {
		cout << "---";
	}
	cout << "---" << endl;
	//line 7
	cout << "HUM: ";
	for(int i=0; i<pits; i++) {
		if (i < 10) {
			cout <<i<<" |";
		} else {
			cout <<i<<"|";
		}
	} cout<<endl;
}

// Function that checks if the game is over. If so, it returns a value of the winner
int MancalaBoard::gameOver(void) const {
	//cout << "checking for gameOver" << endl;
	if (field[pits] > (stones*pits)) return 1; // more than half of the total number of stones already means a win
	if (field[(2*pits+1)] > (stones*pits)) return 2; // more than half of the total number of stones already means a win

	int human = 0, cpu = 0, endgame = 0;
	for (int i=0 ; i<pits ; i++) {
		human += field[i];
	}
	if (human == 0) {
		cpu = 0;
		for (int i=pits+1 ; i<(2*pits+2) ; i++) {
			cpu += field[i];
		}
		endgame = 1;
		human = field[pits];
	}

	for (int i=pits+1 ; i<(2*pits+1) ; i++) {
		cpu += field[i];
	}
	if (cpu == 0) {
		human = 0;
		for (int i=0 ; i<pits+1 ; i++) {
			human += field[i];
		}
		endgame = 1;
		cpu = field[(2*pits+1)];
	}

	if ((human==cpu) && (endgame==1)) return 3; // draw
	if ((cpu>human) && (endgame==1)) return 2; // cpu = winner
	if ((human>cpu) && (endgame==1)) return 1; // human = winner
	return 0; // no winner
}

// returns the number of stones of a pit
int MancalaBoard::getField(int index) const {
	//cout << "getting value of field[" << index << "]" << endl;
	return field[index];
}

// returns a copy of the entire field
MancalaBoard::Field MancalaBoard::getField(void) const {
	//cout << "copying the field..." << endl;
	return field;
}

// sets the number of stones in a pit
void MancalaBoard::setField(int index, int value) {
	//cout << "setField field[" << index << "] with value: " << value << endl;
	field[index] = value;
}

// increment the value of field[index] with the value of add
void MancalaBoard::addField(int index, int add) {
	//cout << "addField field[" << index << "] with add value: " << add << endl;
	int temp = getField(index);
	temp += add;
	setField(index, temp);
}

// constructor of MancalaPlayer
MancalaPlayer::MancalaPlayer(MancalaBoard board, Side side) : side(side), board(board) {}

// Function that sows stones, returns the type of the last pit where a stone was sown in
MancalaPlayer::Lastpit MancalaPlayer::sow(MancalaBoard& board, Side side, int frompit) {
	//cout << "going to sow from pit: " << frompit << endl;
	if ((side == HUMAN) && (frompit > board.getNumberOfPits())) {cout << "Illegal move - sow from your own pits" << endl; return TRYAGAIN;}
	if (frompit < 0) { cout << "Illegal move" << endl; return TRYAGAIN; }
	if ((frompit == board.getNumberOfPits()) || (frompit == (2*board.getNumberOfPits()+1))) { cout << "You cannot sow from a store" << endl; return TRYAGAIN; }
	if (board.getField(frompit) == 0) {cout << "You cannot sow from an empty pit" << endl; return TRYAGAIN;}

	int sowstones = board.getField(frompit);
	int pit = frompit;
	int previouspit = frompit;
	board.setField(frompit, 0); // clear pit from where is sown
	while (sowstones > 0) {
		//cout << "sowstones: " << sowstones << endl;
		// illegal index -1 is used to see if we're going around the field (from highest index to the beginning of the field) while sowing
		if(pit != -1) { // go the the next field index (normal operation)
			previouspit = pit;
			pit++;
		} else if (pit == -1) { // go back to the beginning of the field (index 0)
			previouspit = (2*board.getNumberOfPits()+1);
			pit = 0;
		} else { // error occured
			cout << "Something went wrong with going back to the beginning of the field while sowing stones" << endl;
		}

		if (side == COMPUTER) {
			if (pit == (2*board.getNumberOfPits()+1)) {
				board.addField(pit, 1); // place stone
				sowstones--; // decrement sowstones
				pit = -1; // set pit to -1
			} else {
				board.addField(pit, 1); // place stone
				sowstones--; // decrement sowstones
			}
		} else if (side == HUMAN) {
			if (pit == (2*board.getNumberOfPits()+1)) {
				// don't sow in opponents store
				pit = -1; // set pit to -1
			} else {
				board.addField(pit, 1); // place stone
				sowstones--; // decrement sowstones
			}
		}
	}

	//cout << "done sowing..." << endl;
	// from this point pit is equal to lastpit (no more sowstones)
	// a fix for ending with invalid pit index -1
	if (pit == -1) {
		pit = (2*board.getNumberOfPits()+1);
	}

	// return value
	// if the last pit is filled with only 1 stone and it was not a store it was EMPTY before the sow action
	if ((board.getField(pit) == 1) && !((pit == board.getNumberOfPits()) || (pit == (2*board.getNumberOfPits()+1))) ) {
		// steal!
		steal(board, side, pit);
		return EMPTY;
	}
	// if the last pit is a store, return STORE
	if ( ((side == HUMAN) && (pit == board.getNumberOfPits())) || ((side == COMPUTER) && (pit == (2*board.getNumberOfPits()+1))) ) {
		return STORE;
	}
	// else...
	return PIT;
};

// steal function steals from opposite pit and put these stones and the 'steal stone' itself in the store
void MancalaPlayer::steal(MancalaBoard& board, Side side, int lastpit) {
	int stealpit;
	if (side == COMPUTER) {
		//cout << "CPU is stealing" << endl;
		if (lastpit > board.getNumberOfPits()) {
			stealpit = lastpit - board.getNumberOfPits();
			stealpit = board.getNumberOfPits() - stealpit;
			//cout << "Steal from: " << stealpit << " number: " << board.getField(stealpit) << endl;
			board.addField((2*board.getNumberOfPits()+1), board.getField(stealpit));
			board.addField((2*board.getNumberOfPits()+1), 1);
			board.setField(lastpit, 0);
			board.setField(stealpit, 0);
		}
	} else if (side == HUMAN) {
		//cout << "HUMAN is stealing" << endl;
		if (lastpit < board.getNumberOfPits()) {
			stealpit = board.getNumberOfPits() - lastpit;
			stealpit = board.getNumberOfPits() + stealpit;
			//cout << "Steal from: " << stealpit << " number: " << board.getField(stealpit) << endl;
			board.addField(board.getNumberOfPits(), board.getField(stealpit));
			board.addField(board.getNumberOfPits(), 1);
			board.setField(lastpit, 0);
			board.setField(stealpit, 0);
		}
	}
}


// constructor of Human (creates a MancalaPlayer with Side HUMAN)
Human::Human(MancalaBoard& board) : MancalaPlayer(board, HUMAN), turn(false) {}

// the chooseMove determines the best move, this move (or pit) is stored in bestMove
int Human::chooseMove(int& bestMove, Side side, MancalaBoard board, Tactic tactic) {
	// not implemented for HUMAN
	return 0;
}

// constructor of CPU (creates a MancalaPlayer with Side COMPUTER)
Computer::Computer(MancalaBoard& board) : MancalaPlayer(board, COMPUTER), turn(false), depth(0), step(0) {}

// the chooseMove determines the best move, this move (or pit) is stored in bestMove
int Computer::chooseMove(int& bestMove, Side side, MancalaBoard board, Tactic tactic) {
	//cout << "CPU is in chooseMove" << endl;
	depth++;
	int* a = NULL;
	a = new int[board.getNumberOfPits()];
	MancalaBoard boardcopy = board;
	int dummy; //dummy variable

	//-------------------------------------GREEDY----------------------------------------
	// Finds the maximum number of stones that can be taken in 1 turn
	// The best move will be the move that takes as many as possible stones
	// before it is HUMAN's turn again.
	//-----------------------------------------------------------------------------------
	if (tactic == GREEDY) {
		// apply greedy tactic
		int score = board.getField(2*board.getNumberOfPits()+1); // number of stones in pit
		int computerpits = 0;
		for (int i=board.getNumberOfPits()+1; i<(2*board.getNumberOfPits()+1);i++){ // for each pit determine the movevalue
			int movevalue = score;
			if (board.getField(i) != 0){ // Only sow if there are stones in the pit
				Lastpit reply = sow(boardcopy, side, i);
				movevalue = boardcopy.getField(2*board.getNumberOfPits()+1) - score;
				if (reply == STORE) {
					a[computerpits] = movevalue + chooseMove(dummy, side, boardcopy, GREEDY); // recursive call to chooseMove
				} else {
					a[computerpits] = movevalue;
				}
			} else { // no stones, no sow action -> low movevalue
				a[computerpits] = NO_MOVES;
			}
			computerpits++;
			boardcopy = board; // put back a copy of the board to its original state
		}

	//-------------------------------------LOOKAHEAD-----------------------------------------
	// This algorithm looks ahead MAX_DEPTH number of turns to determine the maximum score
	// The best move(s) is the one that CPU gains to most points and HUMAN the least.
	//---------------------------------------------------------------------------------------
	} else if (tactic == LOOKAHEAD) {
		if (side == HUMAN) {
			int score = board.getField(board.getNumberOfPits()); // number of stones in a pit
			for (int i=0; i<board.getNumberOfPits(); i++){ // determine the movevalue for each pit
				int movevalue = score;
				if (board.getField(i) != 0) { // only sow if there are stones in the pit
					Lastpit reply = sow(boardcopy, side, i);
					movevalue =  boardcopy.getField(board.getNumberOfPits()) - score;
					if (reply == STORE) {
						depth--; // depth (turn) stays the same because we're not switching turns
						step++; // one more step within this turn
						a[i] = movevalue + chooseMove(dummy, HUMAN, boardcopy, LOOKAHEAD); // recursive call to chooseMove
						step--;
						depth++;
					} else {
						if (depth <= MAX_DEPTH) {
							a[i] = movevalue - chooseMove(dummy, COMPUTER, boardcopy, LOOKAHEAD); // recursive call to chooseMove
						} else {
							a[i] = movevalue;
						}
					}
				} else { // no stones, no sow action -> low movevalue
					a[i] = NO_MOVES;
				}
				boardcopy = board; // put back a copy of the board to its original state
			}
		} else if (side == COMPUTER) {
			int score = board.getField(2*board.getNumberOfPits()+1); // number of stones in pit
			int computerpits = 0;
			for (int i=(board.getNumberOfPits()+1); i<(2*board.getNumberOfPits()+1); i++){ // for each pit determine the movevalue
				int movevalue = score;
				if (board.getField(i) != 0){ // only sow if there are stones in the pit
					Lastpit reply = sow(boardcopy, side, i);
					movevalue = boardcopy.getField(2*board.getNumberOfPits()+1) - score;
					if (reply == STORE) {
						depth--; // depth (turn) stays the same because we're not switching turns
						step++; // one more step within this turn
							a[computerpits] = movevalue + chooseMove(dummy, COMPUTER, boardcopy, LOOKAHEAD); // recursive call to chooseMove
						step--;
						depth++;
					} else {
						if (depth <= MAX_DEPTH) {
							a[computerpits] = movevalue - chooseMove(dummy, HUMAN, boardcopy, LOOKAHEAD); // recursive call to chooseMove
						}
						else{
							a[computerpits] = movevalue;
						}
					}
				}
				else{ // no stones, no sow action -> low movevalue
					a[computerpits] = NO_MOVES;
				}
				computerpits++;
				boardcopy = board; // put back a copy of the board to its original state
			}
		}
	}

	int max = INT_MIN;
	for (int i=0; i<board.getNumberOfPits(); i++) {
		if (a[i] > max) {
			max = a[i];
			bestMove = i+board.getNumberOfPits()+1;
		}
		// print the value of each move
		//if ((depth == 1) && (step == 0)) {cout << "value of pit "; if ((i+board.getNumberOfPits()+1) < 10) {cout <<" ";} cout << i+board.getNumberOfPits()+1 << "= " << a[i] << endl;}
	}

	// give the move that creates no more possible moves a very high or very low value depending on the player
	if (max == NO_MOVES) {
		if (side == COMPUTER) {
			if (boardcopy.getField(2*board.getNumberOfPits()+1) > (board.getNumberOfPits() * board.getNumberOfStones()))
				max = CPU_WIN;
			else if (boardcopy.getField(2*board.getNumberOfPits()+1) == (board.getNumberOfPits() * board.getNumberOfStones()))
				max = DRAW;
			else
				max = HUMAN_WIN;
		} else if (side == HUMAN) {
			if (boardcopy.getField(board.getNumberOfPits()) > (board.getNumberOfPits() * board.getNumberOfStones()))
				max = -HUMAN_WIN;
			else if (boardcopy.getField(board.getNumberOfPits()) == (board.getNumberOfPits() * board.getNumberOfStones()))
				max = -DRAW;
			else
				max = -CPU_WIN;
		}
	}

	depth--;
	delete [] a;
	return max;
	//cout << "done choosing a move.." << endl;
}



``````````````````````````````````````mancala.h``````````````````````````````````````````````````````````
#include <vector>

using namespace std;

class MancalaBoard {
public:
	typedef vector<int> Field;

	//constructor
	MancalaBoard(int pits, int stones);

	// public memberfuncties (const)
	void printBoard(void) const;
	int gameOver(void) const;
	int getNumberOfPits(void) const { return pits; };
	int getNumberOfStones(void) const { return stones; };
	int getField(int index) const;
	Field getField(void) const;
	// non-const
	void setField(int index, int value);
	void addField(int index, int add);

private:
	Field field; // vector<int>
	int pits; // number of pits per player
	int stones; // number of stones per pit
};

class MancalaPlayer {
public:
	enum Side { HUMAN, COMPUTER };
	enum Lastpit { TRYAGAIN, STORE, EMPTY, PIT };
	enum Tactic { GREEDY, LOOKAHEAD };

	// constructor
	MancalaPlayer(MancalaBoard board, Side side);

	// get status, virtual functions; each player keeps track of the status itself
	virtual bool getTurn() const = 0;
	virtual void setTurn(bool turn) = 0;
	// the chooseMove functions have a different implementation for HUMAN and CPU
	virtual int chooseMove(int& bestMove, Side side, MancalaBoard board, Tactic tactic) = 0;

	// these functions are the same for every player
	Lastpit sow(MancalaBoard& board, Side side, int frompit);
	void steal(MancalaBoard& board, Side side, int lastpit);

private:
	Side side;
	MancalaBoard board;
};

// de class of Human (derived from MancalaPlayer)
class Human: public MancalaPlayer {
public:
	// constructor
	Human(MancalaBoard& board);

	// think of a move...
	int chooseMove(int& bestMove, Side side, MancalaBoard board, Tactic tactic);

	// get status of the game (who's turn it is)
	bool getTurn() const { return turn; }
	void setTurn(bool t) { if (t==true) { turn=true; } if (t==false) { turn=false; } return;}

private:
	bool turn; // my turn?
};

// class of CPU (derived from MancalaPlayer)
class Computer: public MancalaPlayer {
public:
	// constructor
	Computer(MancalaBoard& board);

	// think of a move...
	int chooseMove(int& bestMove, Side side, MancalaBoard board, Tactic tactic);

	// get status of the game (who's turn it is)
	bool getTurn() const { return turn; }
	void setTurn(bool t) { if (t==true) { turn=true; } if (t==false) { turn=false; } return;}

private:
	bool turn; // my turn?
	int depth; // number of turns that is looked ahead
	int step; // number of steps within a turn that is tried
};

``````````````````````````````````````````main.c```````````````````````````````````````````````````````
#define DEFAULT_PITS 6
#define DEFAULT_STONES 4

#include <iostream>
#include "mancala.h"

using namespace std;

int main() {
	cout << "Mancala - By Kees and Boaz" << endl << endl;
	int pits, stones;
	do {
		cout << "How many pits p? 1<p<8 (for default type 0)" << endl;
		cin >> pits;
		if (pits == 0) {
			pits = DEFAULT_PITS;
		}
	} while (!((pits > 1) && (pits < 8)));
	do {
		cout << "How many stones s? 1<s<7 (for default type 0)" << endl;
		cin >> stones;
		if (stones == 0) {
			stones = DEFAULT_STONES;
		}
	} while (!((stones > 1) && (stones < 7)));

	// create a board
	MancalaBoard mancala(pits, stones);
	// create two players and assign them to the board
	Computer cpuplayer(mancala);
	Human humanplayer(mancala);

	int begin;
	do {
		cout << "Who may begin? 1=HUMAN 2=CPU" << endl;
		cin >> begin;
		if (begin == 1) {
			humanplayer.setTurn(true);
		} else if (begin == 2) {
			cpuplayer.setTurn(true);
		} else {
			cout << "Wrong input, type 1 or 2 and press enter" << endl;
		}
	} while (!(begin == 1 || begin == 2));

	// print the board
	mancala.printBoard();

	do {
		MancalaPlayer::Lastpit reply;
		if (humanplayer.getTurn() == true) {
			int frompit;
			do {
				cout << "HUMAN sow: " << flush;
				cin >> frompit;

				reply = humanplayer.sow(mancala, MancalaPlayer::HUMAN, frompit);
			} while(reply == MancalaPlayer::TRYAGAIN);

			if (reply == MancalaPlayer::STORE) { // LAST SEED IN STORE, GO AGAIN
				cout << "HUMAN gets another turn!" << endl;
			} else {
				humanplayer.setTurn(false);
				cpuplayer.setTurn(true);
			}

		} else if (cpuplayer.getTurn() == true) {
			int frompit;

			// here the you can adjust the tactic (GREEDY / LOOKAHEAD)
			//cpuplayer.chooseMove(frompit, MancalaPlayer::COMPUTER, mancala, MancalaPlayer::GREEDY);
			cpuplayer.chooseMove(frompit, MancalaPlayer::COMPUTER, mancala, MancalaPlayer::LOOKAHEAD);

			cout << "CPU sows " << frompit << endl;

			reply = cpuplayer.sow(mancala, MancalaPlayer::COMPUTER, frompit);
			if (reply == MancalaPlayer::STORE) { // LAST SEED IN STORE, GO AGAIN
				cout << "CPU received another turn!" << endl;
			} else {
				humanplayer.setTurn(true);
				cpuplayer.setTurn(false);
			}
		}
		// print the board
		mancala.printBoard();
	} while (mancala.gameOver() == 0);

	cout << (mancala.gameOver() == 1 ? "The winner is: HUMAN" : mancala.gameOver() == 2 ? "The winner is: CPU" : "Everyone is a winner!: DRAW") << endl;

	cin.get();
	cin.get();
	return 0;
}
