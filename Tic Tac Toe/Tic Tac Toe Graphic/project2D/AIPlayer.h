#pragma once
#include "Player.h"
class AIPlayer :
	public Player
{
public:
	AIPlayer();
	AIPlayer(char aiCode);

	~AIPlayer();
	// Returns best move for the board, based on AI algorithm
	Move GetMove(const board & theBoard);
	void WarnIllegal(Move move);
	void GameResult(char result);
	static const int MINIMAX_WIN_VALUE = (BOARD_SIZE * BOARD_SIZE)+1;

private:


	MinimaxOption Minimax(const board &theBoard, int depth, int valueToBeat = -MINIMAX_WIN_VALUE);
	int ReverseMinimaxValue(int value);
	MinimaxOption Minimax(const board &theBoard);
	int m_depth;
};

