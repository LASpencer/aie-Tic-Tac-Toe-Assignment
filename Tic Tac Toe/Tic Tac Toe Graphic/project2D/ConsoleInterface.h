#pragma once
#include "UserInterface.h"

/* ConsoleInterface class
	User interface for console, using cin and cout streams
*/
class ConsoleInterface :
	public UserInterface

{
public:
	ConsoleInterface(GameController *m_gc);
	~ConsoleInterface();

	Move getMove();
	// prints board to console
	void displayBoard(board &theBoard); 
	void DisplayMainMenu();
	char GetCommand();
	// prints game over message and result of game to console
	void displayGameOver(char result);	
	// display error message
	void WarnIllegal(Move move);	

	char ChangePlayer(char mark);

	static const char COLUMN_DIVIDER[12];
	static const char ROW_DIVIDER[12];
	static const char TITLE[6][80];

};

