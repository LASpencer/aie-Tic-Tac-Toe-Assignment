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

---GameController---
The GameController object creates all the other game objects and calls their methods based on the player's
input. 
Every update and draw, it checks whether a game is being played, and if so whether the game has been ended.
When drawing, it draws the relevant objects and any other graphic elements needed.
When updating, it checks the relevant objects and take approprate action based on user input.
If outside a game, or when the game is over, this is fairly straightforward. Each button drawn to the screen
is checked to see if it was clicked, and if so some code will be executed.
During a game, its behaviour depends on whether a human or AI is going to take a turn.
On a human player's turn, it checks the buttons drawn for the game board, and if the player clicked a button
representing a legal move the game takes a turn.
On an AI player's turn, it waits for a short time before the game takes a turn.

---Player---
The Player virtual class represents a player in the game.
It contains two methods: setGameController to set a pointer to the GameController object, and getMove to allow
the Game object to get that player's chosen move.

1: AIPlayer
The AIPlayer's move is determined by using a minimax algorithm (more specifically, a negamax algorithm using 
alpha-beta pruning). 
The minimax method returns a MinimaxOption struct. This struct contains a move, the move's value, and the move's
width (how many other moves with the same value could also have been made). The move from this structure is 
returned by getMove.
The minimax method checks every possible legal move on the board passed to it, creating a copy of that board
with the possible move made on it and checking the result. If it's a winning move, it's assigned a value of 10.
If it ends the game with a draw, 0. If the game still continues, the method calls itself, passing the board 
with the new move made on it. The value of this move is equal to the value in the MinimaxOption struct returned, 
with its absolute value reduced by 1 and with opposite sign. This means that letting the other player win is 
bad, and forcing your own win is good. Also, by reducing the value by 1, the number of turns until someone wins
is taken into account, so winning quickly or putting off a defeat will be prefered.
The moves are put into an array of MinimaxOptions, sorted by value and width. Width is 

2: HumanPlayer
The HumanPlayer subclass is far less complex. It gets its move by calling the GameController's getMove method,
returning the last legal move clicked on by a user.

III. Acknowledgements
---------------------

warning_beep.wav from Space Shooter Sound Effects by dravenx
Used under CC BY-SA 3.0 https://creativecommons.org/licenses/by-sa/3.0/legalcode