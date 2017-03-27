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

void HumanPlayer::WarnIllegal(Move move)
{
}

void HumanPlayer::GameResult(char result)
{
	//TODO code to tell gameController how the game went, or set internal variable, or redesign whole thing
}
