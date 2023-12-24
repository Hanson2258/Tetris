#pragma once
#include <string>

class Point
{
	// FRIENDS
	friend class TestSuite; // (Allows TestSuite access to private members for testing)

public:
	// Default Constructor
	Point();

	// Constructor with X and Y setup
	Point(const int x, const int y);

	int getX() const;						// Get X value
	int getY() const;						// Get Y value

	void setX(const int x);                 // Set X value
	void setY(const int y);                 // Set Y value
	void setXY(const int x, const int y);   // Set X,Y value

	void swapXY();							// Swap X and Y value

	void multiplyX(const int factor);		// Multiply X by some factor
	void multiplyY(const int factor);		// Multiply Y by some factor

	// Return a string in the form "[x,y]" to represent the state of the Point instance
	std::string toString() const;

private:
	int x; // X-coordinate
	int y; // Y-coordinate
};
