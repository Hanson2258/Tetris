// The Gameboard class encapsulates the functionality of the Tetris game board.
//
// The Gameboard class has no concept of what a tetromino is!
// This is intentional.  It helps to keep the gameboard as modular and as simple
// as possible. The gameboard class simply manages a 2D array of content
// (integers). Each integer of content can represent either an empty spot, or a
// color.  Gameboard member functions operate on that content (particularly on
// rows) to facilitate operations that happen on a Tetris gameboard (checking for
// a complete row, filling a row, copying a row, collapsing rows, etc…).
//
// Actual gameplay is handled by a separate class (which makes use of the gameboard).
// We will handle this in a future lab. To get a sense of the bigger picture and how 
// the gameboard is used : a GridTetromino (a special Tetromino that has knowledge of
// its location on a co-ordinate system) will eventually reach a location on the 
// gameboard where it cannot move down any further (because the points that it would
// occupy already contain content).  At this point, the information a Tetromino 
// contains about its blocks (location and color) will be copied to the gameboard grid.
// We use the gameboard to determine what locations are already occupied as a result of 
// previous tetromino being “locked” (where its blocks are copied to the gameboard).
// 
// Displaying the board(via text output) should show empty spots, and areas where blocks
// of color(content) have been copied("locked") onto the board from tetrominos that have
// already been placed(either intentionally or not).
//
// - The game board is represented by a 2D array of integers(the grid).
// - The array contains content(integers) which represent either :
//    - an EMPTY_BLOCK(-1),
//    - a color from the Tetromino::TetColor enum.
// - This 2D array is oriented with [0][0] at the top left and [MAX_Y][MAX_X] at the
//      bottom right.  Why?  It makes the board much easier to draw the grid on the 
//      screen later because this is the same way things are drawn on a screen 
//      co-ordinate system (where pixel 0,0 is considered top left).
// - A 2D array uses row - major ordering by default – which means that the first index
//      is the row and the second is the column.eg: grid[row][col]. However it will be
//      easier for us to think of this grid in terms of XY coordinates. Here’s the 
//      catch… X represents columns and Y represents rows.  (I know this seems counter-
//      intuitive, but to make sense of it - think of pixels on a screen, or XY 
//      co-ordinates on a co-ordinate system)  
// - This means that when we index into a 2D array with an x, y co - ordinate, we are 
//      going to need to reverse what we would probably do naturally : (grid[x][y]), 
//      and index the grid the other way around(grid[y][x]). We will only need to do 
//      this within the gameboard class (since grid is a private member variable). 
//		We are using abstraction to hide the details of implementation inside the class.
//      If we call member functions that are public (eg : setContent(x, y, content))
//      we can treat X and Y as we normally would,
//
//  [expected .cpp size: ~ 225 lines (including method comments)]

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include "Point.h"

class Gameboard
{

	// FRIENDS
	friend class TestSuite; // (Allows TestSuite access to private members for testing)
	friend int main(); // Allow main() to access private vars (for testing)

	public:
		// CONSTANTS
		static constexpr int MAX_Y{ 19 };		// Gameboard y (rows) dimension
		static constexpr int MAX_X{ 10 };		// Gameboard x (cols) dimension
		static constexpr int EMPTY_BLOCK{ -1 };	// Contents of an empty block

	private:
		// MEMBER VARIABLES -------------------------------------------------

		// The gameboard - a grid of Y (rows) and X (cols) offsets.  
		//  ([0][0] is top left, [MAX_Y-1][MAX_X-1] is bottom right) 
		int grid[MAX_Y][MAX_X];

		// The gameboard offset to spawn a new Tetromino at.
		const Point spawnLoc{ MAX_X / 2, 0 };
		
	public:	
		// METHODS -------------------------------------------------
		 
		// Constructor
		//  - empty() the grid
		Gameboard();

		// Fill the board with EMPTY_BLOCK
		void empty();


		// Prints the grid contents to the console (for debugging purposes)
		void printToConsole() const;


		// Get the content at a given point 
		// Assert the point is valid
		// - param 1: a Point (the XY of the content want to retrieve)
		// - return: an int, the content from the grid at the specified point 
		int getContent(const Point& point) const;				

		// Get the content at a given XY location
		// Assert the point is valid
		// - param 1: an int for X (cols)
		// - param 2: an int for Y (row)
		// - return: an int, the content from the grid at the specified XY
		int getContent(const int x, const int y) const;			


		// Set the content at a given point (ignores invalid points)
		// - param 1: a Point
		// - param 2: an int representing the content we want to set at this location.
		void setContent(const Point& point, const int content);	

		// Set the content at an x,y position (ignores invalid points)
		// - param 1: an int for X (cols)
		// - param 2: an int for Y (row)
		// - param 3: an int representing the content we want to set at this location	
		void setContent(const int x, const int y, const int content);
		
		// Set the content for a set of points (ignores invalid points)
		// - param 1: a vector of Points representing locations
		// - param 2: an int representing the content we want to set.
		void setContent(const std::vector<Point>& locs, const int content);


		// Determine all points passed in are empty (ignores invalid points)
		// - param 1: a vector of Points representing locations to test
		// - return: true if the content at ALL VALID points is EMPTY_BLOCK, false otherwise
		bool areAllLocsEmpty(const std::vector<Point>& locs) const;


		// Removes all completed rows from the board
		// - params: none
		// - return: the count of completed rows removed
		int removeCompletedRows();


		// Get the spawn location
		// - returns: spawnLoc
		Point getSpawnLoc() const;

	private:

		// Determine if a given Point is a valid grid location
		// - param 1: a Point object
		// - return: true if the point is a valid grid location, false otherwise
		bool isValidPoint(const Point& point) const;

		// Determine if a given XY is a valid grid location
		// - param 1: an int representing x
		// - param 2: an int representing y
		// - return: true if the x,y is a valid grid location, false otherwise
		bool isValidPoint(const int x, const int y) const;


		// Return a bool indicating if a given row is full (no EMPTY_BLOCK in the row)
		// Assert the row index is valid
		// - param 1: an int representing the row (Y) index we want to test
		// - return: bool representing if the row is completed
		bool isRowCompleted(const int rowIndex) const;
		
		// Fill a given grid row with specified content
		// - param 1: an int representing a row index
		// - param 2: an int representing content
		void fillRow(const int rowIndex, const int content);
		
		// Scan the board for completed rows.
		// - params: none
		// - return: a vector of completed row indices (integers).
		std::vector<int> getCompletedRowIndices() const;				

		// Copy a source row's contents into a target row.
		// - param 1: an int representing the source row index
		// - param 2: an int representing the target row index
		void copyRowIntoRow(const int sourceRow, const int targetRow);
			
		// Assert the row index is valid
		// Removes a row, and moves all above rows down
		// - param 1: an int representing a row index
		void removeRow(int rowIndex);

		// Given a vector of row indices, remove them. 
		// - param 1: a vector of integers representing row indices to remove
		void removeRows(const std::vector<int>& rowIndices);
};

#endif /* GAMEBOARD_H */

