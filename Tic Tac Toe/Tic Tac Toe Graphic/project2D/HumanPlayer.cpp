#include "HumanPlayer.h"
#include "GameController.h"
#include "Game.h"

HumanPlayer::HumanPlayer()
{
}


HumanPlayer::~HumanPlayer()
{
}

Move HumanPlayer::GetMove(const board & theBoard)
{

	return m_gc->getMove();
}