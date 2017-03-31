#pragma once
#include "Game.h"

class Game;
class GameController;
struct Move;


class Player
{
public:
	Player();
	~Player();

	// Decide what move to make based on board given
	virtual Move getMove(const board &theBoard) = 0;
	void setGameController(GameController *gameController);


	/*	Player type codes
		h = human, other types indicate different AI algorithms/difficulties
	*/
	static const char PLAYER_HUMAN = 'h';
	static const char PLAYER_MINIMAX_FULL = 'a';
	static const char PLAYER_MINIMAX_HARD = 'b';
	static const char PLAYER_MINIMAX_MED = 'c';
	static const char PLAYER_MINIMAX_EASY = 'e';

protected:
	GameController *m_gc;

};

