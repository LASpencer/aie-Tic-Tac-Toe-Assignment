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
To be consistent with Bootstrap, methods are written in lower camel case where possible.

The program is composed of the following classes:

Application2D		A Bootstrap application
Button			A clickable button
Game			An object storing a Tic Tac Toe board and methods for playing
			Tic Tac Toe
GameController		An object controlling the user interface and game loop
Player			A virtual class representing players
    AIPlayer		A player that selects its move with an algorithm
    HumanPlayer		A player given its moves through the user interface

---Application2D---
This class is inherited from Bootstrap's Application class. The program's main method 
creates and runs an Application2D object.
On startup creates a Renderer2D object (m_2dRenderer) to draw onto, and a GameController
object (m_gc) which runs the game.
Each update m_gc's update method is called. Then, if the escape key was 
pressed, or m_gc's m_running flag is set to false, Application2D quits.
Similarly its draw method simply clears the screen and calls m_gc's draw method (passing
a pointer to m_2dRenderer).
The class exists so that code from the earlier console version of the program 
could be more easily reused. Rather than completely rewriting GameController as a subclass 
of Application, or rewriting the other objects which contain pointers to GameController 
objects, GameController was modified to include update and draw methods without completely
implementing Application.

---Button---
This class is used for coloured rectangles drawn to the screen, possibly containing text.
They are used to allow the user to make selections by clicking on them. They change colour
when moused over, so the user can see whether they are able to click a button, and can be
set as "pressed" to change their colour, making the current selection visible.
Buttons are also used for the squares on the Tic Tac Toe board. Those buttons do not contain
text (with marks being rendered using lines and circles), and are set as pressed to indicate
an illegal move.
Buttons are simply part of the user interface and do not directly have any effects. It is the
responsibility of the GameController class to check whether a button was clicked and perform 
whatever action is required.

---Game---
A Game object contains a Tic Tac Toe board as a 3x3 char array, with methods for placing marks
on the board and checking if the move would end the game. The Game class also has several static
methods for determining whether a move would be legal, whose turn it is, if someone has won the
game and so on for a board passed to them.
This means that other objects are able to check the results of a move for any possible board state,
without creating any side effects or relying on hidden variables. This is necessary for the AIPlayer's
MiniMax method, which needs to be able to check all possible moves.

III. Acknowledgements
---------------------

warning_beep.wav from Space Shooter Sound Effects by dravenx
Used under CC BY-SA 3.0 https://creativecommons.org/licenses/by-sa/3.0/legalcode