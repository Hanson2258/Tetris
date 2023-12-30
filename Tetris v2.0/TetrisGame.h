// This class encapsulates the Tetris game and its drawing routines, gameplay, & control logic.

#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Gameboard.h"
#include "GridTetromino.h"


class TetrisGame
{
public:
	// STATIC CONSTANT EXPR ---------------------------------------------------
	static constexpr int NUM_NEXT_SHAPES{ 3 };		// Number of next shapes
	static constexpr int numLevels{ 30 };			// Number of levels
	static constexpr int numGameLoopElements{ 2 };	// secondPerGameLoop, secondsPerTick

	// STATIC CONSTANTS -------------------------------------------------------
	static const int BLOCK_WIDTH;				// Pixel width of a Tetris block
	static const int BLOCK_HEIGHT;				// Pixel height of a Tetris block
	static const int NEXT_SHAPE_Y_SPACE;		// The pixel spacing between the next shapes in the Y column
	static const int pauseTimeAfterShapePlaced;	// Time to pause for after shape has been placed

	// STATIC VARIABLES -------------------------------------------------------
	static bool holdShapeSet;			// True if holdShape has been set this game
	static bool holdShapeSetThisRound;	// True if holdShape has been set this round

	// 2D array to display the secondsPerRound & secondsPerTick per level
	static double gameLoopTime[numLevels][numGameLoopElements];	

private:
	// MEMBER VARIABLES -------------------------------------------------------

	// Gameboard --------------------------------------------------
	Gameboard board;	// The gameboard (grid) to represent where all the blocks are


	// Current, ghost, hold and next shape(s) ---------------------
	GridTetromino currentShape; // The tetromino that is currently falling
	GridTetromino ghostShape;	// A ghost for the tetromino that is currently falling

	// NextShape linked list
	struct NextShapes
	{
		GridTetromino shape;
		Point viewOffset;
		NextShapes* pNext;
	};

	NextShapes* pNextShapeHead;	// Head of nextShape linked list
	NextShapes* pNextShapeTail;	// Tail of nextShape linked list

	GridTetromino holdShape;	// The Tetromino that is on hold


	// Score ------------------------------------------------------
	int score;				 // The current game score
	int level;				 // The current level

	// Scoring points for actions
	enum class scoringActions
	{
		singleRowClear = 100,
		doubleRowClear = 300,
		tripleRowClear = 500,
		Tetris = 800,

		softDrop = 1,
		hardDrop = 2
	};

	int totalRowsCleared{0}; // Total lines cleared


	// Graphics members -------------------------------------------
	sf::RenderWindow& window;				// The window to draw on
	sf::Sprite& blockSprite;				// The sprite used for all the blocks
	const Point gameboardOffset;			// Pixel XY offset of the gameboard on the screen
	const Point holdShapeCenter;			// Pixel XY center of the hold shape area on the screen
	const Point* nextShapeCenter;			// Pixel XY center for nextShape(s)
	Point holdShapeOffset;					// Pixel XY offset for the hold shape area on the screen
	Point nextShapeOffset[NUM_NEXT_SHAPES]; // Pixel XY offset for nextShape(s)

	sf::Font font;			 // SFML font for text
	sf::Text title;			 // SFML text object for displaying the title
	sf::Text holdShapeTitle; // SFML text object for displaying the block in hold
	sf::Text nextShapeTitle; // SFML text object for displaying the next shapes
	sf::Text scoreTitle;	 // SFML text object for displaying the score title
	sf::Text scoreDisplay;	 // SFML text object for displaying the score
	sf::Text levelTitle;	 // SFML text object for displaying the level title
	sf::Text levelDisplay;	 // SFML text object for displaying the level
	sf::Text linesTitle;	 // SFML text object for displaying the lines (rows cleared) title
	sf::Text linesDisplay;	 // SFML text object for displaying the lines (rows cleared)


	// Time members -----------------------------------------------
	// Note: a "tick" is the amount of time it takes a block to fall one line.
	double secondsSinceLastTick{ 0.0 };			// This updates every game loop until it is >= secondPerGameLoop
	double secondsSinceLastPlacement{ 0.0 };	// The amount of time since the last shape placement
	bool shapePlacedSinceLastGameLoop{ false }; // Tracks if a shape has been placed (locked) in the current game loop


	// Audio members ----------------------------------------------
	sf::Music tetrisMusic;
	sf::Music blockDrop;
	sf::Music blockRotate;
	sf::Music levelUp;
	sf::Music gameOver;


public:
	// ========================================================================
	// ============================= Constructor ==============================
	// ========================================================================

	// Constructor
	//  - Initialize/ assign private member vars names that match param names
	//  - Initialize audio files
	//  - Loads font
	//  - reset() the game
	//  - Setup all text
	//
	// - param 1: Render Window object
	// - param 2: Sprite object
	// - param 3: Point object (the offset of the gameboard)
	// - param 4: Point object array (next shape(s) center)
	TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, const Point& gameboardOffset,
		const Point nextShapeCenter[], const Point& holdShapeCenter);


	// ========================================================================
	// ============================ Public Methods ============================
	// ========================================================================

	// Event and game loop processing
	//  - handles keypress events
	//  - update ghost shape
	//
	// Provides controls for the game
	//  - Keyboard::Up    - Attempt to rotate
	//  - Keyboard::Left  - Attempt to move to the left
	//  - Keyboard::Down  - Attempt to soft drop (down one row)
	//  - Keyboard::Right - Attempt to move to the right
	//  - Keyboard::Space - Hard drop
	//  - Keyboard::C     - Attempt to hold shape
	//
	// If attempt is successful, execute.
	//
	// - param 1: sf::Event event
	void onKeyPressed(const sf::Event& event);

	// Called every game loop to handle ticks & tetromino placement (locking)
	// - Pauses after a block is placed for "pauseTimeAfterShapePlaced" time
	// - Calls "tick()" every time "secondsSinceLastTick" exceeds "secondsPerTick"
	// - If shape was placed, spawn next shape(s), check to clear rows, update score,
	//    update level, level display and lines display, and reset all variables
	//    as needed.
	// - If game ends, stop tetris music, play game over music, and sleep for 5 seconds.
	//
	// Note: If move was successful, "secondsSinceLastTick" is reset, but
	//          will still call "tick()" if "secondsSinceLastPlacement" exceeds
	//          secondsPerTick.
	// 
	// - param 1: float secondsSinceLastLoop
	void processGameLoop(float secondsSinceLastLoop);

	// Draw anything to do with the game, which is:
	// For Tetrominos:
	//  - currentShape
	//  - ghostShape
	//  - holdShape
	//  - nextShape(s)
	//
	// The gameboard
	//
	// For texts:
	//  - title
	//  - holdShapeTitle
	//  - nextShapeTitle
	//  - scoreTitle
	//  - scoreDisplay
	//  - levelTitle
	//  - levelDisplay
	//  - linesTitle
	//  - linesDisplay
	//
	// Called every game loop
	void draw() const;



private:
	// ========================================================================
	// =============================== Methods ================================
	// ========================================================================


	// ==============================================================
	// ===================== Game loop methods ======================
	// ==============================================================

	// A tick() forces the currentShape to move (if there were no tick,
	// the currentShape would float in position forever). This should
	// call attemptMove() on the currentShape.  If not successful, lock() 
	// the currentShape (it can move no further).
	void tick();

	// Updates level
	//  - Level = totalRowsCleared / 10 + 1
	//  - plays levelUp music if level up
	void updateLevel();

	// Reset everything for a new game
	//  - reset the score, level, and totalRowsCleared
	//  - update the display for score, level, and lines
	//  - Clear the gameboard
	//  - Delete all shapes in nextShapes linked list
	//  - Pick & spawn next shapes (both the "on-deck" shapes, and nextShapes linked list
	//  - Play tetris music
	void reset();


	// ==============================================================
	// ========================= Set Shapes =========================
	// ==============================================================

	// Populate the nextShapes linked list with "NUM_NEXT_SHAPES" of
	// shapes.
	void setStartingShapes();

	// Create and returns a new shape
	//  - Must return one of each shape, before a new set of all shapes
	//     can be returned
	//
	// - param 1: a bool to determine if reset it needed (such as when the game resets)
	// - returns: NextShapes - a nextShapes object
	NextShapes* addNewShape(bool reset);

	// Copy the nextShape into the currentShape (through assignment)
	//  - Position the currentShape to its spawn location
	//  - Updates the ghost shape
	//
	// - return: bool, true/false based on isPositionLegal()
	bool spawnNextShape();

	// Picks a new shape to put on the end of the nextShapes linked list
	//  - Deletes the head of nextShapes linked list
	//  - Add a new shape to the end of nextShapes linked list
	//  - Update nextShapeOffset array
	void pickNextShape();

	// Sets hold shape
	//  - Sets hold shape to what current shape is
	//    - If there is already a current shape, set current shape to the previous
	//       hold shape
	//    - Else, set current shape to the next shape, and update the nextShapes
	//       linked list accordingly
	void setHoldShape();


	// ==============================================================
	// ========================== Movement ==========================
	// ==============================================================

	// Test if a rotation is legal on the tetromino and if so, rotate it.
	//
	// - param 1: GridTetromino shape
	// - return: bool, true/false to indicate successful movement
	bool attemptRotate(GridTetromino& shape) const;

	// Test if a move is legal on the tetromino, if so, move it.
	//
	// - param 1: GridTetromino shape
	// - param 2: int x (cols)
	// - param 3: int y (rows)
	// - return: true/false to indicate successful movement
	bool attemptMove(GridTetromino& shape, int x, int y) const;

	// Drops the tetromino vertically as far as it can legally go.
	//
	// - param 1: GridTetromino shape
	// - return: int of num rows dropped
	int drop(GridTetromino& shape) const;

	// Copy the contents (color) of the Tetrominos mapped block locs to the grid.
	//
	// - param 1: GridTetromino shape
	// - return: nothing
	void lock(const GridTetromino& shape);

	// Updates ghost shape to be in the same position as current shape, but dropped.
	void updateGhostShape();

	// Delete all shapes in the nextShapes linked list
	void deleteNextShapes();


	// ==============================================================
	// ================== State & gameplay/ logic ===================
	// ==============================================================

	// Determine if a Tetromino can legally be placed at its current position
	// on the gameboard.
	//
	// - param 1: GridTetromino shape
	// - return: bool, true if shape is within borders (isWithinBorders()) and 
	//           the shape's mapped board locs are empty (false otherwise).
	bool isPositionLegal(const GridTetromino& shape) const;

	// Determine if the shape is within the left, right, & bottom gameboard borders
	// Ignores the upper border because we want shapes to be able to drop
	// in from the top of the gameboard.
	// All of a shape's blocks must be inside these 3 borders to return true.
	//
	// - param 1: GridTetromino shape
	// - return: bool, true if the shape is within the left, right, and lower border
	//	         of the grid, but *NOT* the top border (false otherwise)
	bool isWithinBorders(const GridTetromino& shape) const;


	// ==============================================================
	// ========================== Graphics ==========================
	// ==============================================================

	// Draw a Tetris block sprite on the canvas		
	// The block position is specified in terms of 2 offsets: 
	//    1) the top left (of the gameboard in pixels)
	//    2) an x & y offset into the gameboard - in blocks (not pixels)
	//       meaning they need to be multiplied by BLOCK_WIDTH and BLOCK_HEIGHT
	//       to get the pixel offset.
	//
	// - param 1: Point topLeft
	// - param 2: int xOffset (cols)
	// - param 3: int yOffset (rows)
	// - param 4: TetColor color
	void drawBlock(const Point& topLeft, int xOffset, int yOffset, const Tetromino::TetColor& color,
	               float alpha = 1.0f) const;

	// Draw the gameboard blocks on the window
	void drawGameboard() const;

	// Draw a tetromino on the window.
	//
	// - param 1: GridTetromino tetromino
	// - param 2: Point topLeft
	void drawTetromino(const GridTetromino& tetromino, const Point& topLeft, float alpha = 1.0f) const;

	// Update the score display
	//  - Display the current score
	//  - Sets position to center itself depending on the size of the string
	void updateScoreDisplay();

	// Update the level display
	//  - Display the current level
	//  - Sets position to center itself depending on the size of the string
	void updateLevelDisplay();

	// Update the lines (rows cleared) display
	//  - Display the current lines
	//  - Sets position to center itself depending on the size of the string
	void updateLinesDisplay();


	// ==============================================================
	// =========================== Audio ============================
	// ==============================================================

	// Loads all audio files and alter the volume
	void audioSetup();


public:
	// ==============================================================
	// ========================= Destructor =========================
	// ==============================================================

	~TetrisGame();
};

#endif /* TETRISGAME_H */
