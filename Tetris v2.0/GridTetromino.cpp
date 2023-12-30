#include "GridTetromino.h"


// Constructor ----------------------------------------------------------------

GridTetromino::GridTetromino()
{
	setGridLoc(0, 0);
}


// METHODS ----------------------------------------------------------------

// Getters ---------------------------------

float GridTetromino::getXViewBlockOffset() const
{
	return xViewBlockOffset;
}

float GridTetromino::getYViewBlockOffset() const
{
	return yViewBlockOffset;
}

Point GridTetromino::getGridLoc() const
{
	return gridLoc;
}


// Setters ---------------------------------

void GridTetromino::setGridLoc(const int x, const int y)
{
	gridLoc.setXY(x, y);
}

void GridTetromino::setGridLoc(const Point& point)
{
	gridLoc.setXY(point.getX(), point.getY());
}


// Other Methods ---------------------------------

void GridTetromino::move(const int xOffset, const int yOffset)
{
	gridLoc.setXY(gridLoc.getX() + xOffset, gridLoc.getY() + yOffset);
}

std::vector<Point> GridTetromino::getBlockLocsMappedToGrid() const
{
	std::vector<Point> mappedBlockLocs;

	for (int i{0}; i < static_cast<int>(blockLocs.size()); i++)
	{
		mappedBlockLocs.emplace_back(blockLocs[i].getX() + gridLoc.getX(), blockLocs[i].getY() + gridLoc.getY());
	}

	return mappedBlockLocs;
}
