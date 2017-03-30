#pragma once
#include "Player.h"

struct MinimaxOption {
	Move move;
	int value;
	// how many options of same value and depth this was selected from
	size_t width;
};

class AIPlayer :
	public Player
{
public:
	AIPlayer();
	AIPlayer(char aiCode);

	~AIPlayer();
	/* Returns best move for the board
		theBoard = board on which to find best move
	*/
	Move GetMove(const board & theBoard);
	static const int MINIMAX_WIN_VALUE = (BOARD_SIZE * BOARD_SIZE)+1; // Value of winning move

private:

	/* Returns MinimaxOption containing one of the best moves possible on board passed, and its value
		
		theBoard = board on which to find best move
		depth = how many turns after this one to check
		valueToBeat = current best value in MiniMax method calling this one
	*/
	MinimaxOption Minimax(const board &theBoard, int depth, int valueToBeat = -MINIMAX_WIN_VALUE);
	MinimaxOption Minimax(const board &theBoard);

	/* Reduces absolute value by 1 and reverses sign
		value = value to reverse and reduce
	*/
	int ReverseMinimaxValue(int value);
	int m_depth;
};

