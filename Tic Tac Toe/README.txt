TicTacToe by L. Andrew Spencer 2017

This is a program for playing Tic Tac Toe. It supports human vs human, human vs AI 
and AI vs AI games, with various difficulty levels available.

I. Use
-------

The program will display a menu screen with several buttons. The green Play button 
starts a game of Tic Tac Toe, the orange Quit button ends the program, and the red
and Player Selection buttons are used to set the players.
The current selection is highlighted in yellow.

On starting a game, a board of 9 squares is shown. During a human player's turn, 
clicking on an empty square places their mark in that square. If a player attempts
to play in a marked square, it will flash red and play a warning sound.

If either player marks a line of 3 squares, they win. Otherwise the game ends after
all 9 squares are marked.

When the game is over, three buttons are displayed: A green Play button, to play 
another game, a yellow Menu button, to return to the menu screen, and an orange
Quit button, to end the program.


II. Design
----------

The program is composed of the following classes:

Application2D		A Bootstrap application
Button			A clickable button
Game			An object storing a Tic Tac Toe board and methods for playing
			Tic Tac Toe
GameController		An object controlling the user interface and game loop
Player			A virtual class for classes representing players
    AIPlayer		A player that selects its move with an algorithm
    HumanPlayer		A player given its moves through the user interface

---Application2D---



III. Acknowledgements
---------------------

warning_beep.wav from Space Shooter Sound Effects by dravenx
Used under CC BY-SA 3.0 https://creativecommons.org/licenses/by-sa/3.0/legalcode