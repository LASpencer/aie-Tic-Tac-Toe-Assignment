#include <stdexcept>
#include "GameController.h"
#include "Game.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "Renderer2D.h"
#include "Input.h"
#include "Texture.h"

const char GameController::SELECT_PLAYER_TEXT[4][16] = {
	"Human Player","Easy AI","Medium AI","Hard AI"
};

const float GameController::MARK_THICKNESS = 8.0f;
const float GameController::MARK_MARGIN = 5.0f;

GameController::GameController() : m_font("./font/consolas.ttf", 64)	//TODO put into initializer lest where possible
{
	m_xPlayer = new HumanPlayer();
	m_xPlayerType = Player::PLAYER_HUMAN;
	m_xPlayer->setGameController(this);
	m_oPlayer = new HumanPlayer();
	m_oPlayerType = Player::PLAYER_HUMAN;
	m_oPlayer->setGameController(this);
	m_game = new Game(this);
	m_inGame = false;
	m_running = true;
	m_turnTimer = 0;
	m_warningTimer = 0;
	m_menuTitle = new aie::Texture("./textures/tictactoe_title.png");
	m_warningBeep = new aie::Audio("./audio/warning_beep.wav");
	initButtons();
}

GameController::GameController(char xPlayerType, char oPlayerType) : m_font("./font/consolas.ttf", 64)
{
	
	if (xPlayerType == Player::PLAYER_HUMAN) {
		m_xPlayer = new HumanPlayer();
	}
	else {
		m_xPlayer = new AIPlayer(xPlayerType);	
	}
	if (oPlayerType == Player::PLAYER_HUMAN) {
		m_oPlayer = new HumanPlayer();
	}
	else {
		m_oPlayer = new AIPlayer(oPlayerType);	
	}
	m_xPlayer->setGameController(this);
	m_oPlayer->setGameController(this);
	m_xPlayerType = xPlayerType;
	m_oPlayerType = oPlayerType;
	m_game = new Game(this);
	m_inGame = false;
	m_running = true;
	m_turnTimer = 0;
	m_warningTimer = 0;
	m_menuTitle = new aie::Texture("./textures/tictactoe_title.png");
	m_warningBeep = new aie::Audio("./audio/warning_beep.wav");
	initButtons();
}

GameController::~GameController()
{
	delete m_game;
	delete m_xPlayer;
	delete m_oPlayer;
}

void GameController::setXPlayer(Player * theXPlayer)
{
	m_xPlayer = theXPlayer;
}

void GameController::setOPlayer(Player * theOPlayer)
{
	m_oPlayer = theOPlayer;
}

void GameController::setXPlayer(char playerType)
{
	if (m_xPlayer != nullptr) {
		delete m_xPlayer;
	}
	if (playerType == Player::PLAYER_HUMAN) {
		m_xPlayer = new HumanPlayer();
	}
	else {
		m_xPlayer = new AIPlayer(playerType);
	}
	m_xPlayer->setGameController(this);
	m_xPlayerType = playerType;
}

void GameController::setOPlayer(char playerType)
{
	if (m_oPlayer != nullptr) {
		delete m_oPlayer;
	}
	if (playerType == Player::PLAYER_HUMAN) {
		m_oPlayer = new HumanPlayer();
	}
	else {
		m_oPlayer = new AIPlayer(playerType);
	}
	m_oPlayer->setGameController(this);
	m_oPlayerType = playerType;
}

Player * GameController::getPlayer(char mark)
{
	Player* chosenPlayer;
	switch (mark) {
	case 'x':
		chosenPlayer = m_xPlayer;
		break;
	case 'o':
		chosenPlayer = m_oPlayer;
		break;
	default:
		throw std::invalid_argument("Mark must be 'x' or 'o'");
	}
	return chosenPlayer;
}

void GameController::Update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();

	if (m_inGame) {
		if (m_game->IsGameOver()) {
			m_playGameOverButton->Update(deltaTime);
			m_menuGameOverButton->Update(deltaTime);
			m_quitGameOverButton->Update(deltaTime);
			// check for replay or quit button clicks
			if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
				if (m_playGameOverButton->WasClicked()) {
					m_game->Initialize();
				}
				else if(m_menuGameOverButton->WasClicked()) {
					m_inGame = false;
				}
				else if (m_quitGameOverButton->WasClicked()) {
					m_running = false;
				}
			}
		} else{
			//Check whose turn it is
			char mark = m_game->getCurrentTurn();
			char playerType;
			switch (mark) {
			case 'x':
				playerType = m_xPlayerType;
				break;
			case 'o':
				playerType = m_oPlayerType;
				break;
			}
			// If human player's turn
			if (playerType == Player::PLAYER_HUMAN) {
				// Update board buttons
				for (int i = 0; i < BOARD_SIZE; ++i) {
					for (int j = 0; j < BOARD_SIZE; ++j) {
						m_boardButton[i][j]->Update(deltaTime);
					}
				}
				// Remove illegal move warning after time
				m_warningTimer += deltaTime;
				if (m_warningTimer > 0.2) {
					for (int i = 0; i < BOARD_SIZE; ++i) {
						for (int j = 0; j < BOARD_SIZE; ++j) {
							if (m_boardButton[i][j]->IsPressed()) {
								m_boardButton[i][j]->setPressed(false);
							}
						}
					}
				}
				// Check if player clicked square
				board gameBoard;
				m_game->copyGameBoard(gameBoard);
				char mark = m_game->getCurrentTurn();
				if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
					bool buttonClicked = false;
					for (int i = 0; i < BOARD_SIZE && !buttonClicked; ++i) {
						for (int j = 0; j < BOARD_SIZE &&!buttonClicked; ++j) {
							if (m_boardButton[i][j]->WasClicked()) {
								buttonClicked = true;
								Move playerMove = { i, j };
								if (m_game->IsMoveLegal(playerMove, mark, gameBoard)) {
									m_move = playerMove;
									m_game->TakeTurn();
								} else{
									// Illegal move flashes red
									m_warningTimer = 0;
									m_boardButton[i][j]->setPressed(true);
									// Play error noise
									m_warningBeep->play();
								}
							}
						}
					}
				}
			}
			else {
				//If AI player, wait for timer then take turn
				m_turnTimer += deltaTime;
				if (m_turnTimer > 0.5) {
					// Take turn
					m_turnTimer = 0;
					m_game->TakeTurn();
				}
			}
		}
	}
	else {// In main menu
		//Update Buttons
		for (int i = 0; i < 4; i++) {
			m_xPlayerMenuButton[i]->Update(deltaTime);
			m_oPlayerMenuButton[i]->Update(deltaTime);
		}
		m_playButton->Update(deltaTime);
		m_quitButton->Update(deltaTime);

		// Check for button clicks
		if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
			bool buttonClicked = false;

			// Check if xPlayerType buttons clicked
			for (int i = 0; i < 4; i++) {
				if (m_xPlayerMenuButton[i]->WasClicked()) {
					buttonClicked = true;
					// Check button wasn't already clicked
					if (!m_xPlayerMenuButton[i]->IsPressed()) {
						// Change xPlayer to new type
						switch (i) {
						case 0:
							setXPlayer(Player::PLAYER_HUMAN);
							break;
						case 1:
							setXPlayer(Player::PLAYER_MINIMAX_EASY);
							break;
						case 2:
							setXPlayer(Player::PLAYER_MINIMAX_MED);
							break;
						case 3:
							setXPlayer(Player::PLAYER_MINIMAX_HARD);
							break;
						}

						// Unset previously pressed button
						for (int n = 0; n < 4; n++) {
							if (m_xPlayerMenuButton[n]->IsPressed()) {
								m_xPlayerMenuButton[n]->setPressed(false);
							}
						}

						// Set new button as pressed
						m_xPlayerMenuButton[i]->setPressed(true);
					}
					
					break;
				}
			}
			//Check if oPlayerType buttons clicked
			if (!buttonClicked) {
				for (int i = 0; i < 4; i++) {
					if (m_oPlayerMenuButton[i]->WasClicked()) {
						buttonClicked = true;
						// Check button wasn't already clicked
						if (!m_oPlayerMenuButton[i]->IsPressed()) {
							// Change xPlayer to new type
							switch (i) {
							case 0:
								setOPlayer(Player::PLAYER_HUMAN);
								break;
							case 1:
								setOPlayer(Player::PLAYER_MINIMAX_EASY);
								break;
							case 2:
								setOPlayer(Player::PLAYER_MINIMAX_MED);
								break;
							case 3:
								setOPlayer(Player::PLAYER_MINIMAX_HARD);
								break;
							}

							// Unset previously pressed button
							for (int n = 0; n < 4; n++) {
								if (m_oPlayerMenuButton[n]->IsPressed()) {
									m_oPlayerMenuButton[n]->setPressed(false);
								}
							}

							// Set new button as pressed
							m_oPlayerMenuButton[i]->setPressed(true);
						}

						break;
					}
				}
			}

			// Check if play button clicked
			if (!buttonClicked) {
				if (m_playButton->WasClicked()) {
					m_game->Initialize();
					m_inGame = true;
					buttonClicked = true;
				}
			}
			if (!buttonClicked) {
				if (m_quitButton->WasClicked()) {
					m_running = false;
				}
			}
		}
	}

}

void GameController::Draw(aie::Renderer2D &renderer)
{
	//Draw backround
	renderer.setRenderColour(BACKGROUND_COLOUR);
	renderer.drawBox(480, 360, 960, 720);
	if (m_inGame) {
		displayBoard(renderer);
		if (m_game->IsGameOver()) {
			displayGameOver(renderer);
		}
	} else{
		displayMainMenu(renderer);
	}
}

Move GameController::getMove()
{
	return m_move;
}

void GameController::displayBoard(aie::Renderer2D &renderer)
{
	board display;
	m_game->copyGameBoard(display);
	// draw the board
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			m_boardButton[i][j]->Draw(renderer);
			//draw mark
			float markThickness = MARK_THICKNESS;
			float margin = MARK_MARGIN;
			float markPosX = m_boardButton[i][j]->GetPosX();
			float markPosY = m_boardButton[i][j]->GetPosY();
			float markWidth = (m_boardButton[i][j]->GetWidth() - 2*margin);
			float markHeight = (m_boardButton[i][j]->GetHeight() - 2*margin);
			
			unsigned int markColour = MARK_COLOUR;
			switch (display[i][j]) {
			case 'x':
				renderer.setRenderColour(markColour);
				renderer.drawLine(markPosX - (markWidth / 2), markPosY + (markHeight / 2), markPosX + (markWidth / 2), markPosY - (markHeight / 2), markThickness);
				renderer.drawLine(markPosX - (markWidth / 2), markPosY - (markHeight / 2), markPosX + (markWidth / 2), markPosY + (markHeight / 2), markThickness);
				break;
			case 'o':
				renderer.setRenderColour(markColour);
				renderer.drawCircle(markPosX, markPosY, (markWidth / 2));
				renderer.setRenderColour(m_boardButton[i][j]->GetColour());
				renderer.drawCircle(markPosX, markPosY, (markWidth / 2) - markThickness);
				break;
			default:
				break;
			}
		}
	}
	// Print whose turn it is, or game over message
	char turnMessage[12];
	char gameStatus = m_game->getGameStatus();
	char mark;
	switch (gameStatus) {
	case Game::STATUS_PLAYING:
		mark = m_game->getCurrentTurn();
		switch (mark) {
		case 'x':
			strcpy_s(turnMessage, "X's Turn");
			break;
		case 'o':
			strcpy_s(turnMessage, "O's Turn");
			break;
		default:
			strcpy_s(turnMessage, "");
			break;
		}
		break;
	case Game::STATUS_X_WIN:
		strcpy_s(turnMessage, "X Wins");
		break;
	case Game::STATUS_O_WIN:
		strcpy_s(turnMessage, "O Wins");
		break;
	case Game::STATUS_DRAW:
		strcpy_s(turnMessage, "Draw");
		break;
	default:
		strcpy_s(turnMessage, "");
		break;
	}
	
	float messageWidth = renderer.measureTextWidth(&m_font, turnMessage);
	renderer.setRenderColour(TEXT_COLOUR);
	renderer.drawText(&m_font, turnMessage, 445 - (messageWidth/2), 40);
}

void GameController::displayGameOver(aie::Renderer2D &renderer)
{
	char result = m_game->getGameStatus();
	if (result != Game::STATUS_DRAW) {
		// Draw winning line
		Move winningLine[2];
		m_game->getWinningLine(winningLine);
		renderer.setRenderColour(WIN_LINE_COLOUR);
		float winLineStartX = m_boardButton[winningLine[0].row][winningLine[0].col]->GetPosX();
		float winLineStartY = m_boardButton[winningLine[0].row][winningLine[0].col]->GetPosY();
		float winLineEndX = m_boardButton[winningLine[1].row][winningLine[1].col]->GetPosX();
		float winLineEndY = m_boardButton[winningLine[1].row][winningLine[1].col]->GetPosY();
		float winLineThickness = 10;
		renderer.drawLine(winLineStartX,winLineStartY,winLineEndX,winLineEndY,winLineThickness);
	}
	// Draw game over message and quit/replay/menu buttons
	m_playGameOverButton->Draw(renderer);
	m_menuGameOverButton->Draw(renderer);
	m_quitGameOverButton->Draw(renderer);
}

void GameController::displayMainMenu(aie::Renderer2D & renderer)
{
	// Draw title image
	renderer.setRenderColour(0xFFFFFFFF);
	renderer.drawSprite(m_menuTitle, 480, 396, 960,648);
	// Draw buttons
	for (int i = 0; i < 4; i++) {
		m_xPlayerMenuButton[i]->Draw(renderer);
		m_oPlayerMenuButton[i]->Draw(renderer);
	}
	m_playButton->Draw(renderer);
	m_quitButton->Draw(renderer);
}

void GameController::WarnIllegal(Move move)
{
	// Play noise and set background of square to red for a short time
}

void GameController::ChangePlayer(char mark, char playerType)
{
	switch (mark) {
	case 'x':
		setXPlayer(playerType);
		break;
	case 'o':
		setOPlayer(playerType);
		break;
	}
}

char GameController::getPlayerType(char mark)
{
	char playerType = 0;
	switch (mark) {
	case 'x':
		playerType = m_xPlayerType;
		break;
	case 'o':
		playerType = m_oPlayerType;
		break;
	default:
		throw std::invalid_argument("Received mark other than 'x' or 'o'");
	}
	return playerType;
}

void GameController::initButtons()
{
	// Board buttons
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			m_boardButton[i][j] = new Button(245 +(j*200), 595-(i*200), 190, 190);
			m_boardButton[i][j]->setColour(BOARD_COLOUR);
			m_boardButton[i][j]->setHoverColour(BOARD_HOVER_COLOUR);
			m_boardButton[i][j]->setPressedColour(BOARD_WARNING_COLOUR);
		}
	}
	// Buttons to select xPlayer type
	for (int i = 0; i < 4; i++) {
		m_xPlayerMenuButton[i] = new Button(240, 200 - (i * 50), 220, 50);
		m_xPlayerMenuButton[i]->setColour(SELECT_PLAYER_BUTTON_COLOUR);
		m_xPlayerMenuButton[i]->setHoverColour(SELECT_PLAYER_HOVER_BUTTON_COLOUR);
		m_xPlayerMenuButton[i]->setPressedColour(SELECT_PLAYER_PRESSED_BUTTON_COLOUR);
		m_xPlayerMenuButton[i]->setText(SELECT_PLAYER_TEXT[i]);
	}
	int xIndex;
	switch (m_xPlayerType) {
	case Player::PLAYER_HUMAN:
		xIndex = 0;
		break;
	case Player::PLAYER_MINIMAX_EASY:
		xIndex = 1;
		break;
	case Player::PLAYER_MINIMAX_MED:
		xIndex = 2;
		break;
	case Player::PLAYER_MINIMAX_HARD:
		xIndex = 3;
		break;
	}
	m_xPlayerMenuButton[xIndex]->setPressed(true);

	// Buttons to select oPlayer type
	for (int i = 0; i < 4; i++) {
		m_oPlayerMenuButton[i] = new Button(720, 200 - (i * 50), 220, 50);
		m_oPlayerMenuButton[i]->setColour(SELECT_PLAYER_BUTTON_COLOUR);
		m_oPlayerMenuButton[i]->setHoverColour(SELECT_PLAYER_HOVER_BUTTON_COLOUR);
		m_oPlayerMenuButton[i]->setPressedColour(SELECT_PLAYER_PRESSED_BUTTON_COLOUR);
		m_oPlayerMenuButton[i]->setText(SELECT_PLAYER_TEXT[i]);
	}
	int oIndex;
	switch (m_oPlayerType) {
	case Player::PLAYER_HUMAN:
		oIndex = 0;
		break;
	case Player::PLAYER_MINIMAX_EASY:
		oIndex = 1;
		break;
	case Player::PLAYER_MINIMAX_MED:
		oIndex = 2;
		break;
	case Player::PLAYER_MINIMAX_HARD:
		oIndex = 3;
		break;
	}
	m_oPlayerMenuButton[oIndex]->setPressed(true);

	m_playButton = new Button(380, 380, 100, 50);
	m_playButton->setColour(PLAY_BUTTON_COLOUR);
	m_playButton->setHoverColour(PLAY_HOVER_BUTTON__COLOUR);
	m_playButton->setText("Play");
	m_quitButton = new Button(580,380,100,50);
	m_quitButton->setColour(QUIT_BUTTON_COLOUR);
	m_quitButton->setHoverColour(QUIT_HOVER_BUTTON_COLOUR);
	m_quitButton->setText("Quit");
	m_playGameOverButton = new Button(670,60,100,50);
	m_playGameOverButton->setColour(PLAY_BUTTON_COLOUR);
	m_playGameOverButton->setHoverColour(PLAY_HOVER_BUTTON__COLOUR);
	m_playGameOverButton->setText("Play");
	m_menuGameOverButton = new Button(780,60,100,50);
	m_menuGameOverButton->setColour(MENU_BUTTON_COLOUR);
	m_menuGameOverButton->setHoverColour(MENU_HOVER_BUTTON_COLOUR);
	m_menuGameOverButton->setText("Menu");
	m_quitGameOverButton = new Button(890,60,100,50);
	m_quitGameOverButton->setColour(QUIT_BUTTON_COLOUR);
	m_quitGameOverButton->setHoverColour(QUIT_HOVER_BUTTON_COLOUR);
	m_quitGameOverButton->setText("Quit");
}
