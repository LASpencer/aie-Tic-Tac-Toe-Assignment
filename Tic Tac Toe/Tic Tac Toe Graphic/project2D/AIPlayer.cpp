#include "AIPlayer.h"
#include "Game.h"
#include <cstdlib>

bool operator<(const MinimaxOption& a, const MinimaxOption& b) {
	bool result;
	if(a.value == b.value){					// If values equal, compare width
		if (a.value > 0) {					
			result = a.width < b.width;		// If value positive or 0, higher width is better (more ways to force win)
		}
		else {
			result = a.width > b.width;		// If value negative or 0, lower width is better (less ways to lose or force draw)
		}
	}	else {
		result = a.value < b.value;			// If values not equal, compare values
	}
	return result;
}

bool operator>(const MinimaxOption& a, const MinimaxOption& b) {
	bool result;
	if (a.value == b.value) {					// If values equal, compare width
		if (a.value > 0) {					
			result = a.width > b.width;		// If value positive or 0, higher width is better (more ways to win or draw)
		}
		else {
			result = a.width < b.width;		// If value negative, lower width is better (less ways to lose)
		}
	}
	else {
		result = a.value > b.value;			// If values not equal, compare values
	}
	return result;
}

AIPlayer::AIPlayer()
{
	m_depth = 8;
}

AIPlayer::AIPlayer(char aiCode)
{
	switch (aiCode) {
	case (PLAYER_MINIMAX_FULL):
		m_depth = 8;
		break;
	case (PLAYER_MINIMAX_HARD):
		m_depth =8;				// Checks all possible states
		break;
	case(PLAYER_MINIMAX_MED):
		m_depth = 3;			// Checks if fork is possible for either player
		break;
	case(PLAYER_MINIMAX_EASY):
		m_depth = 1;			// Checks if either player could win
		break;
	default:
		m_depth = 9;
	}
}


AIPlayer::~AIPlayer()
{
}

Move AIPlayer::GetMove(const board &theBoard) {
	// Get best MinimaxOption from Minimax function
	MinimaxOption moveChoice = Minimax(theBoard,m_depth);
	// Return move from moveChoice
	return moveChoice.move;
}

void AIPlayer::WarnIllegal(Move move)
{
}

void AIPlayer::GameResult(char result)
{
}

MinimaxOption AIPlayer::Minimax(const board & theBoard, int depth, int valueToBeat ) 
{
	MinimaxOption *optionList = new MinimaxOption[BOARD_SIZE*BOARD_SIZE];	//Holds all possible options
	MinimaxOption nextMove;
	MinimaxOption temp;
	MinimaxOption bestMove;
	board testBoard;
	int optionCount = 0;
	int value;
	bool pruneSubtrees = false;
	char mark = Game::CurrentTurn(theBoard);
	for (unsigned int i = 0; i < BOARD_SIZE; i++) {
		for (unsigned int j = 0; j < BOARD_SIZE; j++) {
			if (Game::IsMoveLegal({ i,j }, mark, theBoard)) {		// First check if move is legal
				Game::copyBoard(testBoard, theBoard);				// Set up testBoard
				testBoard[i][j] = mark;								// Put move on testBoard
				if (Game::HasWon(mark, testBoard)) {
					temp = { { i,j },MINIMAX_WIN_VALUE,1 };			// If winning move, add to list with value 10
				}
				else if (Game::IsFull(testBoard)) {
					temp = { { i,j },0,1 };							// If draw, add to list with value 0
				}
				else if (depth <= 0) {
					temp = { { i,j },0,1 };							// If reached end of depth, add to list with value 0
				}
				else {												// Call Minimax for opponent's move with depth decreased
					if (optionCount == 0) {
						nextMove = Minimax(testBoard, depth - 1);
					}
					else {
						nextMove = Minimax(testBoard, depth - 1, optionList[0].value);	// If any options have been checked, pass best option's value as valueToBeat
					}
					value = ReverseMinimaxValue(nextMove.value);	// Value of this move is opposite of opponent's best move's value
					temp = { { i,j }, value, nextMove.width };
				}
			// Insert into optionList, ordered from best to worst move
			int n = optionCount;
			while (n > 0 && (optionList[n - 1] < temp)) {
				optionList[n] = optionList[n - 1];
				n--;
			}
			optionList[n] = temp;
			++optionCount;
			if (n == 0) {														// If this is new best move
				if (ReverseMinimaxValue(optionList[0].value) < valueToBeat) {	// If best move, on being returned, will be a worse move than the best move of parent
					pruneSubtrees = true;										// Stop considering moves, as other player wouldn't let this happen
					break;
				}
			}
		}
		}
		if (pruneSubtrees) {
			break;
		}
	}
	// Figure out how many moves are equal best
	int width = 1;
	while (width < BOARD_SIZE*BOARD_SIZE && optionList[0].value == optionList[width].value){//&&optionList[0].width == optionList[width].width) { //TODO overload == for MinimaxOption instead
		width++;
	}
	bestMove = optionList[rand() % width];	//Choose random option from among the best
	bestMove.width = width;	
	delete[] optionList;
	return bestMove;
}

int AIPlayer::ReverseMinimaxValue(int value) {
	int reverseValue;
	
	if (value == 0) {
		reverseValue = 0;				// Draw is worth the same to both players
	}
	else if (value < 0) {				// Decrease absolute value by 1, and reverse sign
		reverseValue = -(1 + value);
	}
	else {
		reverseValue = 1 - value;
	}
	return reverseValue;
}

MinimaxOption AIPlayer::Minimax(const board & theBoard)
{
	return Minimax(theBoard, m_depth);
}
