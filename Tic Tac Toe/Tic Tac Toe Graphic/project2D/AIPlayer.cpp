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

Move AIPlayer::getMove(const board &theBoard) {
	// Get best MinimaxOption from minimax function
	MinimaxOption moveChoice = minimax(theBoard,m_depth);
	// Return move from moveChoice
	return moveChoice.move;
}

MinimaxOption AIPlayer::minimax(const board & theBoard, int depth, int valueToBeat ) 
{
	MinimaxOption *optionList = new MinimaxOption[BOARD_SIZE*BOARD_SIZE];	//Holds all possible options
	MinimaxOption nextMove;
	MinimaxOption temp;
	MinimaxOption bestMove;
	board testBoard;
	int optionCount = 0;
	int value;
	bool pruneSubtrees = false;
	char mark = Game::findCurrentTurn(theBoard);
	for (unsigned int i = 0; i < BOARD_SIZE; i++) {
		for (unsigned int j = 0; j < BOARD_SIZE; j++) {
			if (Game::isMoveLegal({ i,j }, mark, theBoard)) {		// First check if move is legal
				Game::copyBoard(testBoard, theBoard);				// Set up testBoard
				testBoard[i][j] = mark;								// Put move on testBoard
				if (Game::hasWon(mark, testBoard)) {
					temp = { { i,j },MINIMAX_WIN_VALUE,1 };			// If winning move, add to list with value 10
				}
				else if (Game::isFull(testBoard)) {
					temp = { { i,j },0,1 };							// If draw, add to list with value 0
				}
				else if (depth <= 0) {
					temp = { { i,j },0,1 };							// If reached end of depth, add to list with value 0
				}
				else {												// Call minimax for opponent's move with depth decreased
					if (optionCount == 0) {
						nextMove = minimax(testBoard, depth - 1);
					}
					else {
						nextMove = minimax(testBoard, depth - 1, optionList[0].value);	// If any options have been checked, pass best option's value as valueToBeat
					}
					value = reverseMinimaxValue(nextMove.value);	// Value of this move is opposite of opponent's best move's value
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
				if (reverseMinimaxValue(optionList[0].value) < valueToBeat) {	// If best move, on being returned, will be a worse move than the best move of parent
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
	// Figure out how many moves are equally good
	int width = 1;					// Moves of equal value
	int equivalentOptions = 1;		// Moves of equal value and width
	while (width < BOARD_SIZE*BOARD_SIZE && optionList[0].value == optionList[width].value){
		if (optionList[0].width == optionList[width].width) {
			++equivalentOptions;
		}
		++width;
	}
	bestMove = optionList[rand() % equivalentOptions];	//Choose random option from among the best
	bestMove.width = width;	
	delete[] optionList;
	return bestMove;
}

int AIPlayer::reverseMinimaxValue(int value) {
	int reverseValue;
	
	if (value == 0) {
		reverseValue = 0;				// draw is worth the same to both players
	}
	else if (value < 0) {				// Decrease absolute value by 1, and reverse sign
		reverseValue = -(1 + value);
	}
	else {
		reverseValue = 1 - value;
	}
	return reverseValue;
}

MinimaxOption AIPlayer::minimax(const board & theBoard)
{
	return minimax(theBoard, m_depth);
}
