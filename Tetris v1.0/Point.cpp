#include "Point.h"

#include <sstream>

// Default Constructor
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


int Point::getX() const
{
	return x;
}
int Point::getY() const
{
	return y;
}


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

void Point::swapXY()
{
	const int oldX{ getX() };

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
