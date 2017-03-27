#pragma once
#include "Font.h"
#include "Renderer2D.h"
#include "Game.h"
#include "Button.h"
#include "Audio.h"

class UserInterface;
class Game;
class Player;

class GameController
{
public:
	GameController();
	GameController(char xPlayerType, char oPlayerType);

	~GameController();
	
	void setXPlayer(Player *theXPlayer);
	void setOPlayer(Player *theOPlayer); 

	void setXPlayer(char playerType);
	void setOPlayer(char playerType);

	//returns pointer to player using selected mark
	Player* getPlayer(char mark);

	void Update(float deltaTime);
	void Draw(aie::Renderer2D & renderer);

	// Get a move from the user interface
	Move getMove();
	
	// Displays the current board
	void displayBoard(aie::Renderer2D & renderer);
	
	// Draw game over message
	void displayGameOver(aie::Renderer2D & renderer);

	// Draw the main menu
	void displayMainMenu(aie::Renderer2D &renderer);

	// Tell user interface to display error message
	void WarnIllegal(Move move);


	// Request type of player to change chosen player
	void ChangePlayer(char mark, char playerType);

	char getPlayerType(char mark);


	//// Play a game of tic-tac-toe, taking turns until game is over
	//void playGame();

	bool IsRunning() {
		return m_running;
	}

	static const char NO_COMMAND = 0;
	static const char START_GAME = 1;
	static const char CHANGE_X_PLAYER = 2;
	static const char CHANGE_O_PLAYER = 3;
	static const char QUIT = 4;

	static const unsigned int BOARD_COLOUR = 0x0000FFFF;
	static const unsigned int BOARD_HOVER_COLOUR = 0x0044FFFF;
	static const unsigned int BOARD_WARNING_COLOUR = 0xFF0000FF;
	static const unsigned int SELECT_PLAYER_BUTTON_COLOUR = 0xFF0000FF;
	static const unsigned int SELECT_PLAYER_HOVER_BUTTON_COLOUR = 0xFF2222FF;
	static const unsigned int SELECT_PLAYER_PRESSED_BUTTON_COLOUR = 0xFFFF00FF;
	static const unsigned int MARK_COLOUR = 0x000000FF;
	static const unsigned int BACKGROUND_COLOUR = 0x000000FF;
	static const unsigned int WIN_LINE_COLOUR = 0xBBBBBBFF;
	static const unsigned int PLAY_BUTTON_COLOUR = 0x00FF00FF;
	static const unsigned int PLAY_HOVER_BUTTON__COLOUR = 0x22FF22FF;
	static const unsigned int QUIT_BUTTON_COLOUR = 0xFF8800FF;
	static const unsigned int QUIT_HOVER_BUTTON_COLOUR = 0xFFAA22FF;
	static const unsigned int MENU_BUTTON_COLOUR = 0xFFFF00FF;
	static const unsigned int MENU_HOVER_BUTTON_COLOUR = 0xFFFF22FF;
	static const unsigned int TEXT_COLOUR = 0xBBBBBBFF;

	static const float MARK_THICKNESS;
	static const float MARK_MARGIN;

	static const char SELECT_PLAYER_TEXT[4][16];
	

private:
	UserInterface *m_ui;
	Game *m_game;
	Player *m_xPlayer;
	Player *m_oPlayer;
	Move m_move;
	char m_xPlayerType;
	char m_oPlayerType;
	float m_turnTimer;
	float m_warningTimer;
	bool m_inGame;
	bool m_running;

	Button* m_boardButton[BOARD_SIZE][BOARD_SIZE];
	Button* m_xPlayerMenuButton[4];
	Button* m_oPlayerMenuButton[4];
	Button* m_playButton;
	Button* m_quitButton;
	Button* m_playGameOverButton;
	Button* m_menuGameOverButton;
	Button* m_quitGameOverButton;

	aie::Texture*	m_menuTitle;
	aie::Audio*		m_warningBeep;

	aie::Font m_font;

	void initButtons();
};

