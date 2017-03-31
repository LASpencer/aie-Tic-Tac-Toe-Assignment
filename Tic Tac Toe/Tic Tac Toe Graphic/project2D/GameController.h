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

	// Creates new Player object pointed to by m_xPlayer
	void setXPlayer(char playerType);
	// Creates new Player object pointed to by m_oPlayer
	void setOPlayer(char playerType);

	//returns pointer to player using selected mark
	Player* getPlayer(char mark);

	void update(float deltaTime);
	void draw(aie::Renderer2D & renderer);

	// Returns move from last board square clicked
	Move getMove();
	
	// Displays the current board
	void displayBoard(aie::Renderer2D & renderer);
	
	// draw game over message
	void displayGameOver(aie::Renderer2D & renderer);

	// draw the main menu
	void displayMainMenu(aie::Renderer2D &renderer);

	// Makes relevant board square flash red and plays a beep
	void warnIllegal(Move move);


	// Request type of player to change chosen player
	void changePlayer(char mark, char playerType);

	// Returns type of player with selected mark
	char getPlayerType(char mark);

	bool isRunning() {
		return m_running;
	}	

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

