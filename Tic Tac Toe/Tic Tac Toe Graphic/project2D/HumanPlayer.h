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
};

