#include "Gameboard.h"

#include <cassert>
#include <iomanip>
#include <iostream>


Gameboard::Gameboard()
{
	empty();
}

void Gameboard::empty()
{
	for (int i{ 0 }; i < MAX_Y; i++) {
		fillRow(i, EMPTY_BLOCK);
	}
}


void Gameboard::printToConsole() const
{
	for (int y{ 0 }; y < MAX_Y; y++) {
		for (int x{ 0 }; x < MAX_X; x++) {

			if (grid[y][x] == EMPTY_BLOCK)
			{
				std::cout << "." << std::setw(2);
			}
			else
			{
				std::cout << grid[y][x] << std::setw(2);
			}

		}

		std::cout << "\n";
	}
}


int Gameboard::getContent(const Point& point) const
{
	assert(isValidPoint(point) && "Invalid Point.");	// Invalid Point

	return grid[point.getY()][point.getX()];
}

int Gameboard::getContent(const int x, const int y) const
{
	assert(isValidPoint(x,y) && "Invalid Point.");	// Invalid Point

	return grid[y][x];
}


void Gameboard::setContent(const Point& point, const int content)
{
	if (isValidPoint(point))
	{
		grid[point.getY()][point.getX()] = content;
	}
}

void Gameboard::setContent(const int x, const int y, const int content)
{
	if (isValidPoint(x, y))
	{
		grid[y][x] = content;
	}
}

void Gameboard::setContent(const std::vector<Point>& locs, const int content)
{
	for (int i{ 0 }; i < static_cast<int>(locs.size()); i++) {
		if (isValidPoint(locs[i].getX(), locs[i].getY()))
		{
			grid[locs[i].getY()][locs[i].getX()] = content;
		}
	}
}


bool Gameboard::areAllLocsEmpty(const std::vector<Point>& locs) const
{
	for (int i{ 0 }; i < static_cast<int>(locs.size()); i++)
	{
		if ((isValidPoint(locs[i])) && (getContent(locs[i]) != EMPTY_BLOCK))
		{
			return false;
		}
	}

	return true;
}


int Gameboard::removeCompletedRows()
{
	const std::vector<int> completedRows = getCompletedRowIndices();

	removeRows(completedRows);

	return static_cast<int>(completedRows.size());
}


Point Gameboard::getSpawnLoc() const
{
	return spawnLoc;
}


bool Gameboard::isValidPoint(const Point& point) const
{
	if (((point.getX() < 0) || (point.getX() >= MAX_X)) ||
		((point.getY() < 0) || (point.getY() >= MAX_Y)))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Gameboard::isValidPoint(const int x, const int y) const
{
	if (((x < 0) || (x >= MAX_X)) ||
		((y < 0) || (y >= MAX_Y)))
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool Gameboard::isRowCompleted(const int rowIndex) const
{
	assert(isValidPoint(0, rowIndex) && "Invalid Row Index.");	// Invalid Row Index

	for (int i{ 0 }; i < MAX_X; i++)
	{
		if (grid[rowIndex][i] == EMPTY_BLOCK)
		{
			return false;
		}
	}

	return true;
}

void Gameboard::fillRow(const int rowIndex, const int content)
{
	for (int i{ 0 }; i < MAX_X; i++)
	{
		grid[rowIndex][i] = content;
	}
}

std::vector<int> Gameboard::getCompletedRowIndices() const
{
	std::vector<int> completedRows;

	for (int y=0; y < MAX_Y; y++)
	{
		if (isRowCompleted(y))
		{
			completedRows.push_back(y);
		}
	}

	return completedRows;
}

void Gameboard::copyRowIntoRow(const int sourceRow, const int targetRow)
{
	for (int y{ 0 }; y < MAX_X; y++) {
		grid[targetRow][y] = getContent(y, sourceRow);
	}
}

void Gameboard::removeRow(int rowIndex)
{
	assert(isValidPoint(0, rowIndex) && "Invalid Row Index.");	// Invalid Row Index

	for (int y{--rowIndex}; y >= 0; y--)
	{
		copyRowIntoRow(y, y+1);

	}

	fillRow(0, EMPTY_BLOCK);
}

void Gameboard::removeRows(const std::vector<int>& rowIndices)
{
	for (int i{ 0 }; i < static_cast<int>(rowIndices.size()); i++)
	{
		removeRow(rowIndices[i]);
	}
}
