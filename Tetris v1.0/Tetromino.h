#pragma once
#include <vector>
#include "Point.h"


class Tetromino
{
	// FRIENDS
	friend class TestSuite; // (Allows TestSuite access to private members for testing)

	public:
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

		// Default Constructor
		// Sets shape to "S" block
		Tetromino();

		TetColor getColor() const;				// Get the Color
		TetShape getShape() const;				// Get the Shape

		// - Set the shape
		// - Set the blockLocs for the shape
		// - Set the color for the shape
		void setShape(const TetShape& shape);

		// Chooses a random shape
		static TetShape getRandomShape(bool reset);


		// Rotate the shape 90 degrees around [0,0] (clockwise)
		// - Note: Tetshape::O will not rotate
		void rotateClockwise();


		// Print a grid to display the current shape
		void printToConsole() const;

		private:
			TetColor color;
			TetShape shape;

		protected:
			std::vector<Point> blockLocs;
};
