#include <sstream>
#include "Point.h"


// Constructor ------------------------------------------------------------

Point::Point()
{
	setX(0);
	setY(0);
}

Point::Point(const int x, const int y)
{
	setX(x);
	setY(y);
}


// METHODS ----------------------------------------------------------------

// Getters ---------------------------------

int Point::getX() const
{
	return x;
}

int Point::getY() const
{
	return y;
}


// Setters ---------------------------------

void Point::setX(const int x)
{
	this->x = x;
}

void Point::setY(const int y)
{
	this->y = y;
}

void Point::setXY(const int x, const int y)
{
	setX(x);
	setY(y);
}


// Other Methods ---------------------------------

void Point::swapXY()
{
	const int oldX{getX()};

	setX(getY());
	setY(oldX);
}


void Point::multiplyX(const int factor)
{
	setX(getX() * factor);
}

void Point::multiplyY(const int factor)
{
	setY(getY() * factor);
}


std::string Point::toString() const
{
	std::stringstream coordinates;

	coordinates << "[" << getX() << "," << getY() << "]\n";

	return coordinates.str();
}
