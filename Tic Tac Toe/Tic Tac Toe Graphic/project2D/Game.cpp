
#include "Game.h"
#include "Player.h"
#include "GameController.h"

Game::Game(GameController *gc)
{
	// initialize empty board
	initialize();
	m_gc = gc;
}


Game::~Game()
{
}

// Empties board and sets gameOver to false
void Game::initialize()
{
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			m_gameBoard[i][j] = 0;
		}
	}
	m_gameOver = false;
	m_gameStatus = STATUS_PLAYING;
}

void Game::takeTurn() {

	char mark = findCurrentTurn(m_gameBoard);
	if (mark == 'x' || mark == 'o') {
		takeTurn(mark);
	}
}

void Game::takeTurn(char mark) 
{
	Move theMove;
	Player * activePlayer;
	Player * passivePlayer;
	switch (mark) {										// set player pointers based on whose turn it is
	case 'x':
		activePlayer = m_gc->getPlayer('x');
		passivePlayer = m_gc->getPlayer('o');
		break;
	case 'o':
		activePlayer = m_gc->getPlayer('o');
		passivePlayer = m_gc->getPlayer('x');
		break;
	}
	theMove = activePlayer->getMove(m_gameBoard);
	if (isMoveLegal(theMove, mark, m_gameBoard)) {
		m_gameBoard[theMove.row][theMove.col] = mark;			// Put mark on the game board
		if (hasWon(mark, m_gameBoard)) {						// Check if active player won
			if (mark == 'x') {
				m_gameStatus = STATUS_X_WIN;
			}
			else {
				m_gameStatus = STATUS_O_WIN;
			}
			m_gameOver = true;
		}
		else if (isFull(m_gameBoard)) {						// Check if board is full
			m_gameStatus = STATUS_DRAW;
			m_gameOver = true;
		}
	}
}

// determines if selected mark has a line of 3
bool Game::hasWon(char mark, board theBoard)
{
	bool row = false;
	bool col = false;
	bool diagonal = false;
	for (int i = 0; i < BOARD_SIZE; i++) {						// check each row
		row = true;												// start by assuming row will win
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (theBoard[i][j] != mark) {						// if any square on row isn't the mark
				row = false;									// row won't win
				break;											// stop checking this row
			}
		}
		if (row) {
			break;												// If a row has won, no need to check further
		}
	}
	if (!row) {
		for (int i = 0; i < BOARD_SIZE; i++) {					// check each column	
			col = true;											// Same logic as for row
			for (int j = 0; j < BOARD_SIZE; j++) {
				if (theBoard[j][i] != mark) {					// Except swap indices being iterated over
					col = false;
					break;
				}
			}
			if (col) {
				break;
			}
		}
	}
	if (!row && !col) {											// check diagonals
		diagonal = true;										// Assume diagonal will win
		for (int i = 0; i < BOARD_SIZE; i++) {	
			if (theBoard[i][i] != mark) {						// Check top left to bottom right
				diagonal = false;								// If any square is not the mark, stop checking this diagonal
				break;
			}
		}
		if (!diagonal) {										// If that diagonal didn't win
			diagonal = true;									// Same as above			
			for (int i = 0; i < BOARD_SIZE; i++) {
				if (theBoard[i][(BOARD_SIZE-1) - i] != mark) {	// Except check top right to bottom left
					diagonal = false;
					break;
				}
			}
		}
	}
	return (row||col||diagonal);								// If any of these stayed true, return true
}

bool Game::isFull(board theBoard)
{
	bool full = true;												// Assume board is full
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (theBoard[i][j] != 'x' && theBoard[i][j] != 'o') {	// If any square doesn't have a mark in it
				full = false;										// Board is not full
				break;												// Stop checking
			}
		}
		if (!full) {
			break;
		}
	}
	return full;
}


char Game::findCurrentTurn(const board &theBoard)
{
	int xCount = 0;
	int oCount = 0;
	char currentTurn;
	for (unsigned int i = 0; i < BOARD_SIZE; i++) {		// i = row index
		for (unsigned int j = 0; j < BOARD_SIZE; j++) {	// j = column index
			if (theBoard[i][j] == 'x') {
				xCount++;
			}
			else if (theBoard[i][j] == 'o') {
				oCount++;
			}
		}
	}
	if (xCount + oCount == BOARD_SIZE*BOARD_SIZE) {
		currentTurn = ERROR_BOARD_FULL;					// if all spaces taken, it can't be anyone's turn as the game's over
	}													// NOTE: filled illegal board will return as filled board
	else if (xCount == oCount) {
		currentTurn = 'x';
	}
	else if (xCount == oCount + 1) {
		currentTurn = 'o';
	}
	else {
		currentTurn = ERROR_ILLEGAL_BOARD;				// board must be invalid for some reason
	}
	return currentTurn;
}

char Game::getCurrentTurn()
{
	return findCurrentTurn(m_gameBoard);
}



bool Game::isMoveLegal(Move theMove, char mark,const board &theBoard) {
	bool legal = true;
	char currentTurn;	// mark of whoever's turn it should be
	int xCount = 0;
	int oCount = 0;
	if (mark != 'x' && mark != 'o') {						// invalid mark
		legal = false;
	} else if (theMove.row >= BOARD_SIZE || theMove.col >= BOARD_SIZE) {	// row or col too large for board
		legal = false;
	}
	else if (theBoard[theMove.row][theMove.col] != 0) {						// location on board already has mark
		legal = false;
	}
	else {													// Check which player should be playing
		currentTurn = findCurrentTurn(theBoard);
		if (currentTurn != mark) {						// Either played out of turn, or an error code
			legal = false;
		}
	}
	return legal;
}

void Game::copyGameBoard(board target)
{
	copyBoard(target, m_gameBoard);
}

void Game::copyBoard(board target, const board &source) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			target[i][j] = source[i][j];
		}
	}
}

bool Game::isGameOver()
{
	return m_gameOver;
}

char Game::getGameStatus()
{
	return m_gameStatus;
}

bool Game::getWinningLine(Move winningLine[2], board theBoard)
{
	bool row = false;
	bool col = false;
	bool diagonal = false;
	char mark;
	for (size_t i = 0; i < BOARD_SIZE; i++) {					// check each row
		row = true;
		mark = theBoard[i][0];
		for (size_t j = 1; j < BOARD_SIZE; j++) {
			if (theBoard[i][j] != mark) {
				row = false;
				break;
			}
		}
		if (row) {												// If row wins
			winningLine[0] = { i,0 };							// Line is from start of row
			winningLine[1] = { i,2 };							// to end of row
			break;
		}
	}
	if (!row) {
		for (size_t i = 0; i < BOARD_SIZE; i++) {				// check each column	
			col = true;
			mark = theBoard[0][i];
			for (size_t j = 1; j < BOARD_SIZE; j++) {
				if (theBoard[j][i] != mark) {
					col = false;
					break;
				}
			}
			if (col) {											// If column wins
				winningLine[0] = { 0,i };						// Line from start of column to end of column
				winningLine[1] = { 2,i };
				break;
			}
		}
	}
	if (!row && !col) {															// check diagonals
		mark = theBoard[1][1];
		if (theBoard[0][0] == mark && theBoard[2][2] == mark) {
			diagonal = true;
			winningLine[0] = { 0,0 };
			winningLine[1] = { 2,2 };
		} else if (theBoard[0][2] == mark && theBoard[2][0] == mark) {
			diagonal = true;
			winningLine[0] = { 0,2 };
			winningLine[1] = { 2,0 };
		}
	}
	return (row || col || diagonal);
}

bool Game::getWinningLine(Move winningLine[2])
{
	if (m_gameStatus == STATUS_X_WIN || m_gameStatus == STATUS_O_WIN) {
		return getWinningLine(winningLine, m_gameBoard);
	}
	else{
		return false;
	}
}
