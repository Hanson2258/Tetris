// The Grid Tetromino class is a subclass of the Gameboard class.

#ifndef GRIDTETROMINO_H
#define GRIDTETROMINO_H

#include "Tetromino.h"


// The GridTetromino extends the basic Tetromino
// Functionality added:
//  - The concept of the Tetrominos location on the gameboard/grid. (gridLoc)
//  - The ability to change a Tetrominos location
//  - The ability to retrieve a vector of Tetromino block locations mapped to the gridLoc.
class GridTetromino : public Tetromino
{
private:
	// The [x (cols), y (rows)] location of this tetromino on the grid/ gameboard.
	// This loc changes each time the tetromino moves
	Point gridLoc;

public:
	// Constructor ------------------------------------------------------------

	// Constructor
	//  - Initialize gridLoc to 0,0
	GridTetromino();


	// METHODS ----------------------------------------------------------------

	// Getters ---------------------------------

	// Get xViewBlockOffset
	float getXViewBlockOffset() const;

	// Get yViewBlockOffset
	float getYViewBlockOffset() const;

	// Get the Tetrominos location
	// - return: a Point (the private member variable gridLoc) 
	Point getGridLoc() const;


	// Setters ---------------------------------

	// Set the Tetrominos location.
	//
	// - param 1: int x (cols)
	// - param 2: int y (rows)
	void setGridLoc(int x, int y);

	// Set the Tetrominos location.
	//
	//  - param 1: a Point (the new gridLoc)
	void setGridLoc(const Point& point);


	// Other Methods ---------------------------------

	// Transpose the gridLoc of this shape
	//	(1,0) represents a move to the right (x+1)
	//	(-1,0) represents a move to the left (x-1)
	//	(0,1) represents a move down (y+1)
	//
	// - param 1: int xOffset, the x (cols) offset (distance) to move
	// - param 2: int yOffset, the y (rows) offset (distance) to move
	void move(int xOffset, int yOffset);

	// Build and return a vector of Points to represent the inherited
	// blockLocs vector mapped to the gridLoc of this object instance.
	//
	//  - return: a vector of Point objects.
	std::vector<Point> getBlockLocsMappedToGrid() const;
};

#endif /* GRIDTETROMINO_H */
