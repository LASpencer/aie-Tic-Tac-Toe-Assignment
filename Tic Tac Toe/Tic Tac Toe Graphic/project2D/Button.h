#pragma once
#include "Renderer2D.h"
#include "Font.h"

/*Button Class
Buttons are coloured rectangles containing text. They are drawn on the screen
and check if they have been moused over or clicked on.
*/
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

	/* Copies string to m_text
		text = null terminated string up to 16 characters long (including terminator)
	*/
	void setText(const char* text);

	// Sets colour of button (as hexadecimal RGB code)
	void setColour(unsigned int colour);

	// Sets hover colour of button (as hexadecimal RGB code)
	void setHoverColour(unsigned int colour);

	// Sets pressed colour of button (as hexadecimal RGB code)
	void setPressedColour(unsigned int colour);

	// Sets button as being pressed, changing its appearance
	void setPressed(bool pressed);

	// Returns true if posX and posY are inside the button's area
	bool inButton(float posX, float posY);

	bool isPressed() {
		return m_pressed;
	}

	// Returns true if mouse position was in the button when last updated
	bool doesMouseHover() {
		return m_hover;
	}

	// Returns true if button was clicked on when last updated
	bool wasClicked() {
		return m_clicked;
	}

	float getPosX() {
		return m_posX;
	}
	float getPosY() {
		return m_posY;
	}
	float getWidth() {
		return m_width;
	}
	float getHeight() {
		return m_height;
	}

	/* Returns colour button is drawn as, based on its current state
	Colour is passed as an unsigned 4 byte integer, representing an RGBA colour value
	*/
	unsigned int getColour();

	// Draws coloured rectangle with text written on it
	void draw(aie::Renderer2D &renderer);

	// Checks if mouse is on the button, and if so whether it was clicked
	void update(float deltaTime);

protected:
	float m_posX;
	float m_posY;
	float m_width;
	float m_height;

	unsigned int m_colour;					// Default button colour
	unsigned int m_pressedColour;			// Button colour if set as pressed
	unsigned int m_hoverColour;				// Button colour when mouse is over button
	unsigned int m_textColour;

	char m_text[16];						// Text to be written on button

	aie::Font m_font;

	bool m_pressed;
	bool m_clicked;
	bool m_hover;
};

