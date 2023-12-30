// The Gameboard class encapsulates the functionality of the Tetris game board.

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include "Point.h"


class Gameboard
{
public:
	// CONSTANTS
	static constexpr int MAX_Y{19};			// Gameboard y (rows) dimension
	static constexpr int MAX_X{10};			// Gameboard x (cols) dimension
	static constexpr int EMPTY_BLOCK{-1};	// Contents of an empty block

private:
	// MEMBER VARIABLES -------------------------------------------------------

	// The gameboard - a grid of Y (rows) and X (cols) offsets
	//  ([0][0] is top left, [MAX_Y-1][MAX_X-1] is bottom right) 
	int grid[MAX_Y][MAX_X];

	// The gameboard offset to spawn a new Tetromino at
	const Point spawnLoc{MAX_X / 2, 0};

public:
	// Constructor ------------------------------------------------------------

	// Constructor
	//  - empty() the grid
	Gameboard();


	// METHODS ----------------------------------------------------------------

	// Getters ---------------------------------

	// Get the content at a given point.
	//  - Assert the point is valid
	//
	// - param 1: a Point (the XY of the content want to retrieve)
	// - return: an int, the content from the grid at the specified point
	int getContent(const Point& point) const;

	// Get the content at a given XY location.
	//  - Assert the point is valid
	//
	// - param 1: an int for X (cols)
	// - param 2: an int for Y (row)
	// - return: an int, the content from the grid at the specified XY
	int getContent(int x, int y) const;


	// Setters ---------------------------------

	// Set the content at a given point (ignores invalid points.)
	//
	// - param 1: a Point
	// - param 2: an int representing the content we want to set at this location.
	void setContent(const Point& point, int content);

	// Set the content at an x,y position (ignores invalid points.)
	//
	// - param 1: an int for X (cols)
	// - param 2: an int for Y (row)
	// - param 3: an int representing the content we want to set at this location	
	void setContent(int x, int y, int content);

	// Set the content for a set of points (ignores invalid points.)
	//
	// - param 1: a vector of Points representing locations
	// - param 2: an int representing the content we want to set.
	void setContent(const std::vector<Point>& locs, int content);


	// Other Methods ---------------------------------

	// Fill the board with EMPTY_BLOCK
	void empty();

	// Determine all points passed in are empty (ignores invalid points.)
	//
	// - param 1: a vector of Points representing locations to test
	// - return: true if the content at ALL VALID points is EMPTY_BLOCK, false otherwise
	bool areAllLocsEmpty(const std::vector<Point>& locs) const;

	// Removes all completed rows from the board.
	//
	// - return: the count of completed rows removed
	int removeCompletedRows();

	// Get the spawn location.
	//
	// - returns: spawnLoc
	Point getSpawnLoc() const;


	// Prints the grid contents to the console (for debugging purposes.)
	void printToConsole() const;


private:
	// PRIVATE METHODS --------------------------------------------------------

	// Determine if a given Point is a valid grid location.
	//
	// - param 1: a Point object
	// - return: true if the point is a valid grid location, false otherwise
	bool isValidPoint(const Point& point) const;

	// Determine if a given XY is a valid grid location.
	//
	// - param 1: an int representing x
	// - param 2: an int representing y
	// - return: true if the x,y is a valid grid location, false otherwise
	bool isValidPoint(int x, int y) const;


	// Return a bool indicating if a given row is full (no EMPTY_BLOCK in the row.)
	//  - Assert the row index is valid
	//
	// - param 1: an int representing the row (Y) index we want to test
	// - return: bool representing if the row is completed
	bool isRowCompleted(int rowIndex) const;

	// Fill a given grid row with specified content.
	//
	// - param 1: an int representing a row index
	// - param 2: an int representing content
	void fillRow(int rowIndex, int content);

	// Scan the board for completed rows.
	//
	// - return: a vector of completed row indices (integers).
	std::vector<int> getCompletedRowIndices() const;

	// Copy a source row's contents into a target row.
	//
	// - param 1: an int representing the source row index
	// - param 2: an int representing the target row index
	void copyRowIntoRow(int sourceRow, int targetRow);

	// Assert the row index is valid.
	//  - Removes a row, and moves all above rows down
	//
	// - param 1: an int representing a row index
	void removeRow(int rowIndex);

	// Given a vector of row indices, remove them.
	//
	// - param 1: a vector of integers representing row indices to remove
	void removeRows(const std::vector<int>& rowIndices);
};

#endif /* GAMEBOARD_H */
