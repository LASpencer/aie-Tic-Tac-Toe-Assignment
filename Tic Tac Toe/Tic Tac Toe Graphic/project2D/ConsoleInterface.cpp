#include "ConsoleInterface.h"
#include "Game.h"
#include "Player.h"
#include <map>
#include <stdio.h>
#include <iostream>
#include <string>

const char ConsoleInterface::COLUMN_DIVIDER[12] = "   |   |   ";
const char ConsoleInterface::ROW_DIVIDER[12] = "---+---+---";
const char ConsoleInterface::TITLE[6][80] = {
	{" _______  _____  _____   _______         _____   _______  ____   ______"},
	{"|__   __||_   _|/ ____| |__   __| /\\    / ____| |__   __|/ __ \\ |  ____|"},
	{"   | |     | | | |         | |   /  \\  | |         | |  | |  | || |__"},
	{"   | |     | | | |         | |  / /\\ \\ | |         | |  | |  | ||  __|"},
	{"   | |    _| |_| |____     | | / ____ \\| |____     | |  | |__| || |____"},
	{"   |_|   |_____|\\_____|    |_|/_/    \\_\\\\_____|    |_|   \\____/ |______|"} };

std::map<char, std::string> PlayerTypeMap = {
	{Player::PLAYER_HUMAN, "Human"},
	{Player::PLAYER_MINIMAX_EASY, "Easy AI"},
	{Player::PLAYER_MINIMAX_MED, "Medium AI"},
	{Player::PLAYER_MINIMAX_HARD, "Hard AI"}
};

ConsoleInterface::ConsoleInterface(GameController *gc)
{
	m_gc = gc;
}


ConsoleInterface::~ConsoleInterface()
{
}

Move ConsoleInterface::getMove()
{
	// TODO let player pick how they want to input moves
	Move theMove;
		std::cout << "Choose your move(column, row): " << std::endl;
		std::cin >> theMove.col >> theMove.row;
	return theMove;
}

void ConsoleInterface::displayBoard(board & theBoard)
{
	//TODO make it look nicer
	// TODO display row and col values
	// TODO Change how board is produced so arbitrary boardsize can be used
	char row[BOARD_SIZE * 4];
	char mark;
	system("CLS");
	for (int i = 0; i < BOARD_SIZE; i++) {
		strcpy_s(row, COLUMN_DIVIDER);
		std::cout << COLUMN_DIVIDER<<std::endl;
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (theBoard[i][j] == 'x') {
				mark = 'X';
			} else if(theBoard[i][j] == 'o'){
				mark = 'O';
			} else{
				mark = ' ';
			}
			row[(j * 4) + 1] = mark;
		}
		std::cout << row << std::endl;
		std::cout << COLUMN_DIVIDER<<std::endl;
		if (i < 2) {
			std::cout << ROW_DIVIDER << std::endl;
		}
	}
}

void ConsoleInterface::DisplayMainMenu()
{
	system("CLS");
	for (int i = 0; i < 6; i++) {
		std::cout << TITLE[i] << std::endl;
	}
	std::cout << std::endl;
}

char ConsoleInterface::GetCommand()
{
	int input = 0;
	char command;
	while (input == 0) {
	std::cout << "1. Play\n";
	std::cout << "2. Select X Player (currently " << PlayerTypeMap[m_gc->getPlayerType('x')] << ")\n";
	std::cout << "3. Select O Player (currently " << PlayerTypeMap[m_gc->getPlayerType('o')] << ")\n";
	std::cout << "5. Quit\n";
	std::cin >> input;
	switch (input) {
	case 1:
		command = GameController::START_GAME;
		break;
	case 2:
		command = GameController::CHANGE_X_PLAYER;
		break;
	case 3:
		command = GameController::CHANGE_O_PLAYER;
		break;
	case 4:
		// TODO call function to change control settings
		command = GameController::NO_COMMAND;
		break;
	case 5:
		command = GameController::QUIT;
		break;
	default:
		input = 0;
		break;
	}
	}
	return command;
}



void ConsoleInterface::displayGameOver(char result)
{
	std::cout << "Game over" << std::endl;
	switch (result) {
	case (Game::STATUS_X_WIN):
		std::cout << "X wins" << std::endl;
		break;
	case(Game::STATUS_O_WIN):
		std::cout << "O wins" << std::endl;
		break;
	case(Game::STATUS_DRAW):
		std::cout << "It was a draw" << std::endl;
		break;
	default:
		break;
	}
	system("pause");
}

void ConsoleInterface::WarnIllegal(Move move)
{
	std::cout << "Illegal move" << std::endl;
}

char ConsoleInterface::ChangePlayer(char mark)
{
	int input = 0;
	char playerType;
	while (input == 0) {
		std::cout << "1. " << PlayerTypeMap[Player::PLAYER_HUMAN] << "\n2. " << PlayerTypeMap[Player::PLAYER_MINIMAX_EASY];
		std::cout << "\n3. " << PlayerTypeMap[Player::PLAYER_MINIMAX_MED] << "\n4. " << PlayerTypeMap[Player::PLAYER_MINIMAX_HARD] << "\n";
		std::cin >> input;
		switch (input) {
		case 1:
			playerType = Player::PLAYER_HUMAN;
			break;
		case 2:
			playerType = Player::PLAYER_MINIMAX_EASY;
			break;
		case 3:
			playerType = Player::PLAYER_MINIMAX_MED;
			break;
		case 4:
			playerType = Player::PLAYER_MINIMAX_HARD;
			break;
		default:
			input = 0;
			break;
		}
	}
	return playerType;
}
