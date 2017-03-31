#include "HumanPlayer.h"
#include "GameController.h"
#include "Game.h"

HumanPlayer::HumanPlayer()
{
}


HumanPlayer::~HumanPlayer()
{
}

Move HumanPlayer::getMove(const board & theBoard)
{

	return m_gc->getMove();
}