#include "Tetromino.h"

#include <iostream>

// Initializing the static variables
bool Tetromino::chosenShapes[];  // A bool array of size TetShape::COUNT
int Tetromino::numChosenShapes;	// Num of shapes chosen

Tetromino::Tetromino()
{
	setShape(TetShape::S);
}

Tetromino::TetColor Tetromino::getColor() const
{
	return color;
}

Tetromino::TetShape Tetromino::getShape() const
{
	return shape;
}

Tetromino::TetShape Tetromino::getRandomShape(bool reset)
{
	if (numChosenShapes == static_cast<int>(TetShape::COUNT))
	{
		for (int i = 0, arrayLength = std::size(chosenShapes); i < arrayLength; i++)
		{
			chosenShapes[i] = false;
		}

		numChosenShapes = 0;
	}

	int randNum{ rand() % static_cast<int>(TetShape::COUNT) };

	while (chosenShapes[randNum])
	{
		if (randNum == static_cast<int>(TetShape::COUNT) - 1)
		{
			randNum = 0;
		}
		else
		{
			randNum++;
		}
		
	}

	chosenShapes[randNum] = true;
	numChosenShapes++;

 	return static_cast<TetShape>(randNum);
}


void Tetromino::setShape(const TetShape& shape)
{
	this->shape = shape;

	switch (this->shape)
	{
		case TetShape::S:
			blockLocs = {
				Point(-1, 0),
				Point( 0, 0),
				Point( 0, 1),
				Point( 1, 1)
			};
			color = TetColor::RED;

			xViewBlockOffset = 0.5;
			yViewBlockOffset = 1;

			break;

		case TetShape::Z:
			blockLocs = {
				Point(-1, 1),
				Point( 0, 1),
				Point( 0, 0),
				Point( 1, 0)
			};
			color = TetColor::GREEN;

			xViewBlockOffset = 0.5;
			yViewBlockOffset = 1;

			break;

		case TetShape::J:
			blockLocs = {
				Point(-1,-1),
				Point(-1, 0),
				Point( 0, 0),
				Point( 1, 0)
			};
			color = TetColor::BLUE_DARK;

			xViewBlockOffset = 0.5;
			yViewBlockOffset = 0;

			break;

		case TetShape::L:
			blockLocs = {
				Point(-1, 0),
				Point( 0, 0),
				Point( 1, 0),
				Point( 1,-1)
			};
			color = TetColor::ORANGE;

			xViewBlockOffset = 0.5;
			yViewBlockOffset = 0;

			break;

		case TetShape::O:
			blockLocs = {
				Point(-1, 0),
				Point(-1, 1),
				Point( 0, 1),
				Point( 0, 0)
			};
			color = TetColor::YELLOW;

			xViewBlockOffset = 0;
			yViewBlockOffset = 1;

			break;

		case TetShape::I:
			blockLocs = {
				Point(-2, 0),
				Point(-1, 0),
				Point(0, 0),
				Point(1,0)
			};
			color = TetColor::BLUE_LIGHT;

			xViewBlockOffset = 0;
			yViewBlockOffset = 0.5;

			break;

		case TetShape::T:
			blockLocs = {
				Point(-1, 0),
				Point( 0, 0),
				Point( 1, 0),
				Point( 0,-1)
			};
			color = TetColor::PURPLE;

			xViewBlockOffset = 0.5;
			yViewBlockOffset = 0;

			break;

		default:
			std::cout << "\n--------------------------"
				"		  \nSOMETHING EXTREMELY BROKEN"
				"         \n---------------------------\n";
	}
}


void Tetromino::rotateClockwise()
{
	if (shape != TetShape::O)
	{
		for (auto& blockLoc : blockLocs)
		{
			blockLoc.swapXY();
			blockLoc.multiplyY(-1);
		}
	} 
}

void Tetromino::printToConsole() const
{
	for (int y{ 3 }; y >= -3; y--) {
		for (int x{ -3 }; x <= 3; x++) {

			for (int i{ 0 }; i < static_cast<int>(blockLocs.size()); i++) {
				if ((blockLocs[i].getX() == x) && (blockLocs[i].getY() == y))
				{
					std::cout << "X";

					break;
				}

				if (i == static_cast<int>(blockLocs.size()) - 1)
				{
					std::cout << ".";
				}
			}

		}

		std::cout << "\n";
	}
}