// The Tetromino class encapsulates the functionality of Tetris Tetrominos

#pragma once

#include <vector>
#include "Point.h"


class Tetromino
{
public:
	// ENUM CLASS -------------------------------------------------------------

	// Enum Tetromino Color for each color possible for the Tetrominos
	enum class TetColor
	{
		RED,
		ORANGE,
		YELLOW,
		GREEN,
		BLUE_LIGHT,
		BLUE_DARK,
		PURPLE,
		COUNT
	};

	// Enum Tetromino Shape for each shape possible for the Tetrominos
	enum class TetShape
	{
		S,
		Z,
		L,
		J,
		O,
		I,
		T,
		COUNT
	};


private:
	// MEMBER VARIABLES -------------------------------------------------------

	TetColor color;	// Tetromino Color
	TetShape shape;	// Tetromino Shape


	// STATIC VARIABLES -------------------------------------------------------

	// A bool array of size TetShape::COUNT
	static bool chosenShapes[static_cast<int>(TetShape::COUNT)];

	// Num of shapes chosen
	static int numChosenShapes;										


public:
	// Constructor ------------------------------------------------------------

	// Constructor
	//  - Sets shape to "S" block
	Tetromino();


	// METHODS ----------------------------------------------------------------

	// Getters ---------------------------------

	TetColor getColor() const; // Get the Color
	TetShape getShape() const; // Get the Shape


	// Other Methods ---------------------------------

	// Chooses a random shape.
	//  - Choose a shape that hasn't been chosen yet, when all shapes has been
	//     chosen, all shapes can be chosen again
	//
	// - param 1: a bool to determine if reset it needed (such as when the game resets)
	static TetShape getRandomShape(bool reset);

	// Set the shape.
	//  - Set the shape
	//  - Set the blockLocs for the shape
	//  - Set the color for the shape
	//  - Set the X and Y offset for the shape
	//
	// - param 1: shape - the shape to set
	void setShape(const TetShape& shape);


	// Rotate the shape 90 degrees around [0,0] (clockwise)
	// - Note: TetShape::O will not rotate
	void rotateClockwise();


	// Print a grid to display the current shape
	void printToConsole() const;


protected:
	// PROTECTED MEMBER VARIABLES ---------------------------------------------
	std::vector<Point> blockLocs;

	// Offset for X and Y when viewing (for either being on hold, or on next shapes)
	float xViewBlockOffset;
	float yViewBlockOffset;
};
