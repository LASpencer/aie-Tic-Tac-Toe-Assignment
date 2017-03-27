
#include "Game.h"
#include "Player.h"
#include "GameController.h"
#include <stdexcept>

Game::Game(GameController *gc)
{
	// initialize empty board
	Initialize();
	m_gc = gc;
}


Game::~Game()
{
}

// Empties board and sets gameOver to false
void Game::Initialize()
{
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			m_gameBoard[i][j] = 0;
		}
	}
	m_gameOver = false;
	m_gameStatus = STATUS_PLAYING;
}

void Game::TakeTurn() {

	char currentTurn = CurrentTurn(m_gameBoard);
	if (currentTurn == 'x' || currentTurn == 'o') {
		TakeTurn(currentTurn);
	}
}

void Game::TakeTurn(char mark) 
{
	Move theMove;
	bool legalMove = false;
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
	default:
		throw std::invalid_argument("Received mark other than 'x' or 'o'");	
		break;
	}
	while (!legalMove) {								// Ask for move from player until legal
		theMove = activePlayer->GetMove(m_gameBoard);		
		legalMove = IsMoveLegal(theMove, mark, m_gameBoard);
		if (!legalMove) {
			activePlayer->WarnIllegal(theMove);
		}
	}
	m_gameBoard[theMove.row][theMove.col] = mark;			// Put mark on the game board
	if (HasWon(mark, m_gameBoard)) {						// Check if active player won and inform both players if so
		activePlayer->GameResult(Player::RESULT_WIN);
		passivePlayer->GameResult(Player::RESULT_LOSS);
		if (mark == 'x') {
			m_gameStatus = STATUS_X_WIN;
		} else{
			m_gameStatus = STATUS_O_WIN;
		}
		m_gameOver = true;
	}
	else if (IsFull(m_gameBoard)) {						// Check if board is full and inform both players if so
		activePlayer->GameResult(Player::RESULT_DRAW);
		passivePlayer->GameResult(Player::RESULT_DRAW);
		m_gameStatus = STATUS_DRAW;
		m_gameOver = true;
	}
}

// determines if selected mark has a line of 3
bool Game::HasWon(char mark, board theBoard)
{
	bool row = false;
	bool col = false;
	bool diagonal = false;
	for (int i = 0; i < BOARD_SIZE; i++) {													// check each row
		row = true;
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (theBoard[i][j] != mark) {
				row = false;
				break;
			}
		}
		if (row) {
			break;
		}
	}
	if (!row) {
		for (int i = 0; i < BOARD_SIZE; i++) {												// check each column	
			col = true;
			for (int j = 0; j < BOARD_SIZE; j++) {
				if (theBoard[j][i] != mark) {
					col = false;
					break;
				}
			}
			if (col) {
				break;
			}
		}
	}
	if (!row && !col) {																				// check diagonals
		diagonal = true;
		for (int i = 0; i < BOARD_SIZE; i++) {
			if (theBoard[i][i] != mark) {
				diagonal = false;
				break;
			}
		}
		if (!diagonal) {
			diagonal = true;
			for (int i = 0; i < BOARD_SIZE; i++) {
				if (theBoard[i][(BOARD_SIZE-1) - i] != mark) {
					diagonal = false;
					break;
				}
			}
		}
	}
	return (row||col||diagonal);
}

// Returns true if all spaces on board are marked 
bool Game::IsFull(board theBoard)
{
	bool full = true;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (theBoard[i][j] != 'x' && theBoard[i][j] != 'o') {
				full = false;
				break;
			}
		}
		if (!full) {
			break;
		}
	}
	return full;
}

/*	counts x and o marks on board
returns x if values equal, o if one more x, or error code if board isn't suitable
*/
char Game::CurrentTurn(const board &theBoard)
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
	return CurrentTurn(m_gameBoard);
}



// determines if a move can be legally made
// true if row and col within bounds, no other move made at location, and mark fits turn order
bool Game::IsMoveLegal(Move theMove, char mark,const board &theBoard) {
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
		currentTurn = CurrentTurn(theBoard);
		if (currentTurn != mark) {						// Either played out of turn, or an error code
			legal = false;
			if (currentTurn == ERROR_ILLEGAL_BOARD) {
				throw std::invalid_argument("Illegal board passed to function");
			}
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

bool Game::IsGameOver()
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
	for (size_t i = 0; i < BOARD_SIZE; i++) {													// check each row
		row = true;
		mark = theBoard[i][0];
		for (size_t j = 1; j < BOARD_SIZE; j++) {
			if (theBoard[i][j] != mark) {
				row = false;
				break;
			}
		}
		if (row) {
			winningLine[0] = { i,0 };
			winningLine[1] = { i,2 };
			break;
		}
	}
	if (!row) {
		for (size_t i = 0; i < BOARD_SIZE; i++) {												// check each column	
			col = true;
			mark = theBoard[0][i];
			for (size_t j = 1; j < BOARD_SIZE; j++) {
				if (theBoard[j][i] != mark) {
					col = false;
					break;
				}
			}
			if (col) {
				winningLine[0] = { 0,i };
				winningLine[1] = { 2,i };
				break;
			}
		}
	}
	if (!row && !col) {																				// check diagonals
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
