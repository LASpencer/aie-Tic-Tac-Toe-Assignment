#pragma once
#include "Renderer2D.h"
#include "Font.h"

class Button
{
public:
	Button();
	Button(float posX, float posY, float width, float height);
	~Button();

	// Sets position of button's center
	void setPosition(float posX, float posY);
	// Sets width and height of button
	void setDimensions(float width, float height);

	void setText(const char* text);

	// Sets colour of button (as hexadecimal RGB code)
	void setColour(unsigned int colour);

	// Sets hover colour of button (as hexadecimal RGB code)
	void setHoverColour(unsigned int colour);

	// Sets pressed colour of button (as hexadecimal RGB code)
	void setPressedColour(unsigned int colour);

	void setPressed(bool pressed);

	// Returns true if posX and posY are inside the button's area
	bool InButton(float posX, float posY);

	bool IsPressed() {
		return m_pressed;
	}

	bool DoesMouseHover() {
		return m_hover;
	}

	bool WasClicked() {
		return m_clicked;
	}

	float GetPosX() {
		return m_posX;
	}
	float GetPosY() {
		return m_posY;
	}
	float GetWidth() {
		return m_width;
	}
	float GetHeight() {
		return m_height;
	}
	unsigned int GetColour();

	void Draw(aie::Renderer2D &renderer);

	void Update(float deltaTime);

protected:
	float m_posX;
	float m_posY;
	float m_width;
	float m_height;

	unsigned int m_colour;
	unsigned int m_pressedColour;
	unsigned int m_hoverColour;
	unsigned int m_textColour;

	char m_text[16];

	aie::Font m_font;

	bool m_pressed;
	bool m_clicked;
	bool m_hover;
};

