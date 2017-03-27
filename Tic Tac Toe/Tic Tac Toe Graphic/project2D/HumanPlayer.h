#pragma once
#include "Player.h"
class HumanPlayer :
	public Player
{
public:
	HumanPlayer();
	~HumanPlayer();

	// Tell GameController to prompt user for a move
	Move GetMove(const board &theBoard);

	// Tell GameController to warn user their move is illegal
	void WarnIllegal(Move move);
	void GameResult(char result);
};

