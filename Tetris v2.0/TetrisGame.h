// This class encapsulates the Tetris game and its drawing routines, gameplay, & control logic.

#ifndef TETRISGAME_H
#define TETRISGAME_H

#include "Gameboard.h"
#include "GridTetromino.h"
#include <SFML/Graphics.hpp>


class TetrisGame
{
	public:
		// STATIC CONSTANTS
		static const int BLOCK_WIDTH;			  // Pixel width of a Tetris block, init to 32
		static const int BLOCK_HEIGHT;			  // Pixel height of a Tetris block, int to 32
		static const double MAX_SECONDS_PER_TICK; // The slowest "tick" rate (in seconds), init to 0.75
		static const double MIN_SECONDS_PER_TICK; // The fastest "tick" rate (in seconds), init to 0.20

	private:	
		// MEMBER VARIABLES

		// State members ---------------------------------------------
		int score;					// The current game score.
	    Gameboard board;			// The gameboard (grid) to represent where all the blocks are
	    GridTetromino nextShape;	// The tetromino shape that is "on deck"
	    GridTetromino currentShape;	// The tetromino that is currently falling

		// Scoring points for actions
		enum class scoringActions
		{
			singleRowClear = 100,
			doubleRowClear = 300,
			tripleRowClear = 500,
			Tetris         = 800,

			softDrop       =   1,
			hardDrop       =   2
		};
		
		// Graphics members ------------------------------------------
		sf::RenderWindow& window;		// The window that we are drawing on
		sf::Sprite& blockSprite;		// The sprite used for all the blocks
		const Point gameboardOffset;	// Pixel XY offset of the gameboard on the screen
		const Point nextShapeOffset;	// Pixel XY offset to the nextShape

		sf::Font scoreFont;				// SFML font for displaying the score
		sf::Text scoreText;				// SFML text object for displaying the score
										
		// Time members ----------------------------------------------
		// Note: a "tick" is the amount of time it takes a block to fall one line.
		double secondsPerTick = MAX_SECONDS_PER_TICK; // The seconds per tick (changes depending on score)	

		double secondsSinceLastTick{ 0.0 };			  // This updates every game loop until it is >= secsPerTick,

		bool shapePlacedSinceLastGameLoop{ false };	  // Tracks if a shape has been placed (locked) in the current gameloop	

	public:
		// MEMBER FUNCTIONS

		// Constructor
		// - Initialize/ assign private member vars names that match param names
		// - reset() the game
		// - Loads font from file: fonts/RedOctober.ttf
		// - Setup scoreText
		// - param 1: Render Window object
		// - param 2: Sprite object
		// - param 3: Point object (the offset of the gameboard)
		// - param 4: Point object (the next shape's offset)
		TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, const Point& gameboardOffset, const Point& nextShapeOffset);

		// Draw anything to do with the game, includes:
		// board, currentShape, nextShape, score
		// - Called every game loop
		void draw() const;								

		// Event and game loop processing
		// - handles keypress events (up, left, right, down, space)
		// - param 1: sf::Event event
		// - return: nothing
		void onKeyPressed(const sf::Event& event);

		// Called every game loop to handle ticks & tetromino placement (locking)
		// - param 1: float secondsSinceLastLoop
		void processGameLoop(const float secondsSinceLastLoop);

		// A tick() forces the currentShape to move (if there were no tick,
		// the currentShape would float in position forever). This should
		// call attemptMove() on the currentShape.  If not successful, lock() 
		// the currentShape (it can move no further).
		void tick();

	private:
		// Reset everything for a new game
		//  - Set the score to 0 and call updateScoreDisplay()
		//  - Call determineSecondsPerTick() to determine the tick rate.
		//  - Clear the gameboard,
		//  - Pick & spawn next shape
		//  - Pick next shape again (for the "on-deck" shape)
		void reset();

		// Assign nextShape.setShape a new random shape  
		void pickNextShape();
		
		// Copy the nextShape into the currentShape (through assignment)
		// Position the currentShape to its spawn location.
		// - return: bool, true/false based on isPositionLegal()
		bool spawnNextShape();																	

		// Test if a rotation is legal on the tetromino and if so, rotate it. 
		// - param 1: GridTetromino shape
		// - return: bool, true/false to indicate successful movement
		bool attemptRotate(const GridTetromino& shape);
	   
		// Test if a move is legal on the tetromino, if so, move it.
		// - param 1: GridTetromino shape
		// - param 2: int x (cols)
		// - param 3: int y (rows)
		// - return: true/false to indicate successful movement
		bool attemptMove(const GridTetromino& shape, const int x, const int y);

		// Drops the tetromino vertically as far as it can legally go.
		// (Originally void, changed to int for better scoring.)
		// - param 1: GridTetromino shape
		// - return: int of num rows dropped
		int drop(const GridTetromino& shape);

		// Copy the contents (color) of the tetromino's mapped block locs to the grid.
		// - param 1: GridTetromino shape
		// - return: nothing
		void lock(const GridTetromino& shape);



		// ====================================
		// ========= Graphics methods =========
		// ====================================

		// Draw a Tetris block sprite on the canvas		
		// The block position is specified in terms of 2 offsets: 
		//    1) the top left (of the gameboard in pixels)
		//    2) an x & y offset into the gameboard - in blocks (not pixels)
		//       meaning they need to be multiplied by BLOCK_WIDTH and BLOCK_HEIGHT
		//       to get the pixel offset.
		// - param 1: Point topLeft
		// - param 2: int xOffset (cols)
		// - param 3: int yOffset (rows)
		// - param 4: TetColor color
		void drawBlock(const Point& topLeft, const int xOffset, const int yOffset, const Tetromino::TetColor& color) const;
											
		// Draw the gameboard blocks on the window
		void drawGameboard() const;
		
		// Draw a tetromino on the window
		// - param 1: GridTetromino tetromino
		// - param 2: Point topLeft
		void drawTetromino(const GridTetromino& tetromino, const Point& topLeft) const;
		
		// Update the score display
		// Forms a string "score: ##" to display the current score
		void updateScoreDisplay();



		// ======================================================
		// =========== State & gameplay/logic methods ===========
		// ======================================================

		// Determine if a Tetromino can legally be placed at its current position
		// on the gameboard.
		// - param 1: GridTetromino shape
		// - return: bool, true if shape is within borders (isWithinBorders()) and 
		//           the shape's mapped board locs are empty (false otherwise).
		bool isPositionLegal(const GridTetromino& shape) const;
		
		// Determine if the shape is within the left, right, & bottom gameboard borders
		// Ignores the upper border because we want shapes to be able to drop
		// in from the top of the gameboard.
		// All of a shape's blocks must be inside these 3 borders to return true
		// - param 1: GridTetromino shape
		// - return: bool, true if the shape is within the left, right, and lower border
		//	         of the grid, but *NOT* the top border (false otherwise)
		bool isWithinBorders(const GridTetromino& shape) const;

		// Set secsPerTick based on current score
		// Speed increases by 0.055 ((MAX_SECONDS_PER_TICK - MIN_SECONDS_PER_TICK) * 0.1)
		// every 3,000 points.
		void determineSecondsPerTick();
};

#endif /* TETRISGAME_H */
