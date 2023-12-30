// The Point class encapsulates the functionality of a grid point system.

#pragma once

#include <string>

class Point
{
public:
	// Constructor ------------------------------------------------------------

	// Constructor
	Point();

	// Constructor with X and Y setup
	Point(int x, int y);


	// METHODS ----------------------------------------------------------------

	// Getters ---------------------------------

	int getX() const; // Get X value
	int getY() const; // Get Y value


	// Setters ---------------------------------

	void setX(int x);			// Set X value
	void setY(int y);			// Set Y value
	void setXY(int x, int y);	// Set X,Y value


	// Other Methods ---------------------------------

	void swapXY();				// Swap X and Y value

	void multiplyX(int factor); // Multiply X by some factor
	void multiplyY(int factor); // Multiply Y by some factor

	// Return a string in the form "[x,y]" to represent the state of the Point instance
	std::string toString() const;

private:
	int x; // X-coordinate
	int y; // Y-coordinate
};
