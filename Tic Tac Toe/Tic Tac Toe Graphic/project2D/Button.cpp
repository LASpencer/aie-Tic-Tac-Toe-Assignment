#include "Button.h"
#include "Input.h"
#include "Font.h"

Button::Button() : m_font("./font/consolas.ttf", 32), m_posX(0), m_posY(0),m_height(0),m_width(0),m_pressed(false), m_hover(false),
m_clicked(false),m_colour(0x00000000),m_hoverColour(0x00000000),m_pressedColour(0x00000000), m_textColour(0x000000FF), m_text("")
{
	//strcpy_s(m_text, "");
}

Button::Button(float posX, float posY, float width, float height) : m_font("./font/consolas.ttf", 32), m_posX(posX), m_posY(posY), m_height(height), m_width(width), m_pressed(false), m_hover(false),
m_clicked(false), m_colour(0x00000000), m_hoverColour(0x00000000), m_pressedColour(0x00000000), m_textColour(0x000000FF), m_text("")
{
}


Button::~Button()
{

}

void Button::setPosition(float posX, float posY)
{
	m_posX = posX;
	m_posY = posY;
}

void Button::setDimensions(float width, float height)
{
	m_width = width;
	m_height = height;
}

void Button::setText(const char* text)
{
	strcpy_s(m_text, 16, text);
}

void Button::setColour(unsigned int colour)
{
	m_colour = colour;
}

void Button::setHoverColour(unsigned int colour)
{
	m_hoverColour = colour;
}

void Button::setPressedColour(unsigned int colour)
{
	m_pressedColour = colour;
}

void Button::setPressed(bool pressed)
{
	m_pressed = pressed;
}

bool Button::InButton(float posX, float posY)
{
	if (posX < m_posX + (m_width / 2) && posX > m_posX - (m_width / 2) && posY < m_posY + (m_height / 2) && posY > m_posY - (m_height / 2)) {
		return true;
	} else{
		return false;
	}
}

unsigned int Button::GetColour()
{
	unsigned int colour;
	if (m_pressed) {
		colour = m_pressedColour;
	}
	else if (m_hover) {
		colour = m_hoverColour;
	}
	else {
		colour = m_colour;
	}
	return colour;
}

void Button::Draw(aie::Renderer2D & renderer)
{
	renderer.setRenderColour(GetColour());
	renderer.drawBox(m_posX, m_posY, m_width, m_height);
	if (strcmp(m_text, "") !=0) {
		renderer.setRenderColour(m_textColour);
		float textWidth = renderer.measureTextWidth(&m_font, m_text);
		renderer.drawText(&m_font, m_text, m_posX - textWidth / 2, m_posY - 14);
	}
}

void Button::Update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();
	// Get mouse position
	float mouseX = input->getMouseX();
	float mouseY = input->getMouseY();
	// Check if mouse on button
	if (InButton(mouseX, mouseY)) {
		m_hover = true;
		// Check if button clicked
		if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT)) {
			m_clicked = true;
		}
		else {
			m_clicked = false;
		}
	}	else {
		m_hover = false;
		m_clicked = false;
	}

}
