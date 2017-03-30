#pragma once

class GameController;
class Player;

const size_t BOARD_SIZE = 3;

// define board as 3x3 array of characters
typedef char board[BOARD_SIZE][BOARD_SIZE];

// Move contains index to location on board
struct Move {
	unsigned int row;
	unsigned int col;
};

/* Game class
Contains methods to make moves on board, check if they are legal, and whether they would result in a win or draw
Instance of Game class has a board and pointers to two players.
*/
class Game
{
public:
	Game(GameController * gc);
	~Game();

	// Fills board with null values and sets gameOver to false
	void initialize();

	// Decide whose turn it is and ask them for their move
	void takeTurn();

	/* Get relevant player's move, place it on the board, and check if they ended the game
	mark = 'x' or 'o', representing whether X or O is taking their turn.
	*/
	void takeTurn(char mark);

	// determines if selected mark has a line of 3
	static bool hasWon(char mark, board theBoard);

	// returns true if all spaces on board are marked
	static bool isFull(board theBoard);

	/*	counts x and o marks on board
		returns x if values equal, o if one more x, or error code if board isn't suitable
	*/
	static char findCurrentTurn(const board &theBoard);

	// returns current turn for m_gameBoard
	char getCurrentTurn();


	/* determines if a move can be legally made
	 true if row and col within bounds, no other move made at location, and mark fits turn order
	 theMove = move to be checked
	 mark = 'x' or 'o' representing who's making the move
	 theBoard = reference to board being played on
	 */
	static bool isMoveLegal(Move theMove, char mark, const board &theBoard);

	// Copies m_gameBoard to target board
	void copyGameBoard(board target);

	static void copyBoard(board target, const board & source);

	// Checks if m_gameOver member is set true
	bool isGameOver();

	// Checks current gameStatus: x win, o win, draw or still playing
	char getGameStatus();

	// Sets start and end points of line that wins game on board passed, or returns false if nobody won
	bool getWinningLine(Move winningLine[2], board theBoard);
	//  Sets start and end points of line that wins game on m_gameBoard, or returns false if nobody won
	bool getWinningLine(Move winningLine[2]);

	static const char ERROR_BOARD_FULL = 'f';
	static const char ERROR_INDEX_OOB = 'i'; //index out of bounds
	static const char ERROR_WRONG_TURN = 't';
	static const char ERROR_ILLEGAL_BOARD = 'b';
	static const char STATUS_X_WIN = 'x';
	static const char STATUS_O_WIN = 'o';
	static const char STATUS_DRAW = 'd';
	static const char STATUS_PLAYING = 'p';

private:
	board m_gameBoard;
	GameController* m_gc;
	bool m_gameOver;
	char m_gameStatus;
};

