#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "GameController.h"
#include "Player.h"


Application2D::Application2D() {

}

Application2D::~Application2D() {

}

bool Application2D::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	m_gc = new GameController(Player::PLAYER_HUMAN, Player::PLAYER_MINIMAX_EASY);


	m_cameraX = 0;
	m_cameraY = 0;
	m_timer = 0;

	return true;
}

void Application2D::shutdown() {
	
	//delete m_font;
	delete m_gc;

}

void Application2D::update(float deltaTime) {

	m_timer += deltaTime;

	// get input
	aie::Input* input = aie::Input::getInstance();

	m_gc->Update(deltaTime);
	if (!m_gc->IsRunning()) {
		quit();
	}
	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	m_gc->Draw(*m_2dRenderer);

	// done drawing sprites
	m_2dRenderer->end();
}