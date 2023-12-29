#include "TetrisGame.h"

#include <cassert>
#include <iostream>
#include <thread>

#include "DebugNewOp.h"


// Initializing the static variables
const int TetrisGame::BLOCK_WIDTH{ 32 };
const int TetrisGame::BLOCK_HEIGHT{ 32 };
const double TetrisGame::MAX_SECONDS_PER_TICK{ 0.75 };
const double TetrisGame::MIN_SECONDS_PER_TICK{ 0.20 };
const int TetrisGame::NEXT_SHAPE_Y_SPACE{ 95 };
bool TetrisGame::holdShapeSet = false;
bool TetrisGame::holdShapeSetThisRound = false;

// ====================================
// ========= Member Functions =========
// ====================================

TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, const Point& gameboardOffset, 
						const Point nextShapeCenter[], const Point& holdShapeCenter)
    : window(window), blockSprite(blockSprite), gameboardOffset(gameboardOffset),
		nextShapeCenter(nextShapeCenter), holdShapeCenter(holdShapeCenter)
{
	reset();

	// Setup our font for drawing the score
	if (!font.loadFromFile("fonts/times new roman.ttf"))
	{
		assert(false && "Missing font: times new roman.ttf");
	}

	// -- Set up text
	// Title
	title.setString("Tetris v2.0");
	title.setFont(font);
	title.setCharacterSize(35);
	title.setFillColor(sf::Color::White);
	title.setPosition(491 - (title.getLocalBounds().width/2), 75 - (title.getLocalBounds().height));
	
	// Hold
	hold.setString("Hold");
	hold.setFont(font);
	hold.setCharacterSize(25);
	hold.setFillColor(sf::Color::White);
	hold.setPosition(180 - (hold.getLocalBounds().width / 2), 152 - (hold.getLocalBounds().height));

	// Next Block
	nextBlock.setString("Next Block");
	nextBlock.setFont(font);
	nextBlock.setCharacterSize(25);
	nextBlock.setFillColor(sf::Color::White);
	nextBlock.setPosition(802 - (nextBlock.getLocalBounds().width / 2), 152 - (nextBlock.getLocalBounds().height));

	// Score Title
	scoreTitle.setString("Score");
	scoreTitle.setFont(font);
	scoreTitle.setCharacterSize(25);
	scoreTitle.setFillColor(sf::Color::White);
	scoreTitle.setPosition(187 - (scoreTitle.getLocalBounds().width / 2), 622 - (scoreTitle.getLocalBounds().height));

	// Score Display
	scoreDisplay.setFont(font);
	scoreDisplay.setCharacterSize(18);
	scoreDisplay.setFillColor(sf::Color::White);
	scoreDisplay.setPosition(188 - (scoreDisplay.getLocalBounds().width / 2), 656 - (scoreDisplay.getLocalBounds().height));
}


void TetrisGame::draw() const
{
	drawTetromino(currentShape, gameboardOffset);

	// NextShape* pHead = pNextShape;
	// for (int i = 0; i < NUM_NEXT_SHAPES; i++)
	// {
	// 	drawTetromino(pNextShape->shape, nextShapeOffset[i]);
	//
	// 	pHead = pHead->pNext;
	// }
	// delete pHead;

	if (holdShapeSet)
	{
		drawTetromino(holdShape, holdShapeOffset);
	}
	drawTetromino(pNextShapeHead->shape, nextShapeOffset[0]);
	drawTetromino(pNextShapeHead->pNext->shape, nextShapeOffset[1]);
	drawTetromino(pNextShapeHead->pNext->pNext->shape, nextShapeOffset[2]);
	drawGameboard();

	window.draw(title);
	window.draw(hold);
	window.draw(nextBlock);
	window.draw(scoreTitle);
	window.draw(scoreDisplay);
}

void TetrisGame::onKeyPressed(const sf::Event& event)
{
	switch (event.key.code)
	{
		case sf::Keyboard::Up:
			attemptRotate(currentShape);
			break;

		case sf::Keyboard::Left:
			attemptMove(currentShape,-1,0);
			break;

		case sf::Keyboard::Down:
			if (!attemptMove(currentShape,0,1))
			{
				lock(currentShape);
			}
			else
			{
				// Successful soft drop, increase score
				score += static_cast<int>(scoringActions::softDrop);
				updateScoreDisplay();
			}
			break;

		case sf::Keyboard::Right:
			attemptMove(currentShape,1,0);
			break;

		case sf::Keyboard::C:
			setHoldShape();
			break;

		case sf::Keyboard::Space:
			score += drop(currentShape) * static_cast<int>(scoringActions::hardDrop);
			updateScoreDisplay();
			lock(currentShape);
			break;

		default:
			assert("Something extremely broken!");  // NOLINT(clang-diagnostic-string-conversion)
			break;
	}

	// Update score display location based on score
	scoreDisplay.setPosition(188 - (scoreDisplay.getLocalBounds().width / 2), 656 - (scoreDisplay.getLocalBounds().height));
}


void TetrisGame::processGameLoop(const float secondsSinceLastLoop)
{
	secondsSinceLastTick += secondsSinceLastLoop;

	if (secondsSinceLastTick > secondsPerTick)
	{
		tick();
		secondsSinceLastTick = 0;
	}

	if (shapePlacedSinceLastGameLoop)
	{
		holdShapeSetThisRound = false;

		if (spawnNextShape())
		{
			pickNextShape();

			const int rowsCleared = board.removeCompletedRows();

			switch (rowsCleared)
			{
				case (4):
					score += static_cast<int>(scoringActions::Tetris);
					updateScoreDisplay();
					break;

				case (3):
					score += static_cast<int>(scoringActions::tripleRowClear);
					updateScoreDisplay();
					break;

				case (2):
					score += static_cast<int>(scoringActions::doubleRowClear);
					updateScoreDisplay();
					break;

				case (1):
					score += static_cast<int>(scoringActions::singleRowClear);
					updateScoreDisplay();
					break;

				default:
					break;
			}
		}
		else
		{
			// // 5 Second pause after game end
			// std::this_thread::sleep_for(std::chrono::seconds(5));

			reset();
		}

		shapePlacedSinceLastGameLoop = false;
	}
}

void TetrisGame::tick()
{
	if(!attemptMove(currentShape,0,1))
	{
		lock(currentShape);
	}
}



// ====================================
// ========= Public functions =========
// ====================================

void TetrisGame::reset()
{
	score = 0;
	updateScoreDisplay();

	determineSecondsPerTick();

	board.empty();

	pNextShapeHead = nullptr;
	pNextShapeTail = nullptr;

	setStartingShapes();
	spawnNextShape();
	pickNextShape();
}

void TetrisGame::setHoldShape()
{
	// Hold shape has not yet been set this round
	if (!holdShapeSetThisRound)
	{
		// If Hold shape has been set before
		if (holdShapeSet)
		{
			GridTetromino temp = holdShape;

			holdShape.setShape(currentShape.getShape());
			currentShape.setShape(temp.getShape());
			currentShape.setGridLoc(board.getSpawnLoc().getX(), board.getSpawnLoc().getY());
		}
		// If Hold shape has never been set before
		else
		{
			holdShapeSet = true;
		
			holdShape.setShape(currentShape.getShape());
			spawnNextShape();
		}

		holdShapeOffset = Point{ static_cast<int>(holdShapeCenter.getX() - (holdShape.getXViewBlockOffset() * BLOCK_WIDTH)),
							     static_cast<int>(holdShapeCenter.getY() - (holdShape.getYViewBlockOffset() * BLOCK_HEIGHT)) };
		holdShapeSetThisRound = true;
	}
}

void TetrisGame::setStartingShapes()
{
	for (int i = 0; i < NUM_NEXT_SHAPES; i++)
	{
		NextShapes* pNextShape = addNewShape();

		// If pNextShapeHead is nullptr
		if (!pNextShapeHead)
		{
			pNextShapeHead = pNextShape;
			pNextShapeTail = pNextShape;
		}
		else
		{
			pNextShapeTail->pNext = pNextShape;
			pNextShapeTail = pNextShape;
		}
	}
}

TetrisGame::NextShapes* TetrisGame::addNewShape()
{
	NextShapes* pNextShape = new NextShapes;
	pNextShape->shape.setShape(Tetromino::getRandomShape());
	pNextShape->viewOffset = Point{ static_cast<int>(pNextShape->shape.getXViewBlockOffset() * BLOCK_WIDTH),
									static_cast<int>(pNextShape->shape.getYViewBlockOffset() * BLOCK_HEIGHT) };
	pNextShape->pNext = nullptr;

	return pNextShape;
}

void TetrisGame::pickNextShape()
{
	// Hold the address for pNextShapeHead
	NextShapes* pTemp = pNextShapeHead;

	// Move pHead to pNext
	pNextShapeHead = pNextShapeHead->pNext;

	// Delete pNextShapeHead
	delete pTemp;

	// Add a shape to the tail
	pTemp = addNewShape();

	pNextShapeTail->pNext = pTemp;
	pNextShapeTail = pNextShapeTail->pNext;

	// Recalculate y-axis for all shapes
	pTemp = pNextShapeHead;
	for (int i = 0; i < NUM_NEXT_SHAPES; i++)
	{
		nextShapeOffset[i] = Point{ nextShapeCenter[i].getX() - pTemp->viewOffset.getX(),
									nextShapeCenter[i].getY() - pTemp->viewOffset.getY() };

		pTemp = pTemp->pNext;
	}
}

bool TetrisGame::spawnNextShape()
{
	currentShape.setShape(pNextShapeHead->shape.getShape());
	currentShape.setGridLoc(board.getSpawnLoc().getX(), board.getSpawnLoc().getY());

	return isPositionLegal(currentShape);
}


bool TetrisGame::attemptRotate(const GridTetromino& shape)
{
	GridTetromino tempTetromino{ shape };
	tempTetromino.rotateClockwise();

	if (isPositionLegal(tempTetromino))
	{
		currentShape.rotateClockwise();

		return true;
	}

	return false;
}

bool TetrisGame::attemptMove(const GridTetromino& shape, const int x, const int y)
{
	GridTetromino tempTetromino{ shape };
	tempTetromino.move(x, y);

	if (isPositionLegal(tempTetromino))
	{
		currentShape.move(x, y);

		return true;
	}

	return false;
}

int TetrisGame::drop(const GridTetromino& shape)
{
	int rowsDropped{ 0 };

	while (attemptMove(shape, 0, 1))
	{
		rowsDropped++;
	}

	return rowsDropped;
}

void TetrisGame::lock(const GridTetromino& shape)
{
	const std::vector<Point> tetrominoLoc{ shape.getBlockLocsMappedToGrid() };

	for (int i{ 0 }; i < static_cast<int>(tetrominoLoc.size()); i++)
	{
		board.setContent(tetrominoLoc[i], static_cast<int>(shape.getColor()));
	}

	shapePlacedSinceLastGameLoop = true;
}



// ====================================
// ========= Graphics methods =========
// ====================================

void TetrisGame::drawBlock(const Point& topLeft, const int xOffset, const int yOffset, const Tetromino::TetColor& color) const
{
	sf::Texture texture;
	texture.loadFromFile("./images/tiles.png", sf::IntRect(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT));

	blockSprite.setTextureRect(sf::IntRect(0 + (static_cast<int>(color) * BLOCK_WIDTH), 0, BLOCK_WIDTH, BLOCK_HEIGHT));
	blockSprite.setPosition(static_cast<float>(topLeft.getX()) + static_cast<float>(BLOCK_WIDTH  * xOffset),
							static_cast<float>(topLeft.getY()) + static_cast<float>(BLOCK_HEIGHT * yOffset));

	window.draw(blockSprite);
}

void TetrisGame::drawGameboard() const
{
	for (int y{ 0 }; y < Gameboard::MAX_Y; y++) {
		for (int x{ 0 }; x < Gameboard::MAX_X; x++) {

			if (board.getContent(x,y) != Gameboard::EMPTY_BLOCK)
			{
				drawBlock(gameboardOffset, x, y, static_cast<Tetromino::TetColor>(board.getContent(x,y)));
			}
		}
	}
}

void TetrisGame::drawTetromino(const GridTetromino& tetromino, const Point& topLeft) const
{
	const std::vector<Point> tetrominoLoc{ tetromino.getBlockLocsMappedToGrid() };

	for (int i{ 0 }; i < static_cast<int>(tetrominoLoc.size()); i++)
	{
		drawBlock(topLeft, tetrominoLoc[i].getX(), tetrominoLoc[i].getY(), tetromino.getColor());
	}
}

void TetrisGame::updateScoreDisplay()
{
	scoreDisplay.setString(std::to_string(score));
}



// ======================================================
// =========== State & gameplay/logic methods ===========
// ======================================================

bool TetrisGame::isPositionLegal(const GridTetromino& shape) const
{
	if (board.areAllLocsEmpty(shape.getBlockLocsMappedToGrid()) && isWithinBorders(shape))
	{
		return true;
	}

	return false;

}

bool TetrisGame::isWithinBorders(const GridTetromino& shape) const
{
	const std::vector<Point> tetrominoLoc{ shape.getBlockLocsMappedToGrid() };

	for (int i{ 0 }; i < static_cast<int>(tetrominoLoc.size()); i++)
	{
		if ((tetrominoLoc[i].getX() < 0) || (tetrominoLoc[i].getX() >= Gameboard::MAX_X)
			|| (tetrominoLoc[i].getY() >= Gameboard::MAX_Y))
		{
			return false;
		}
	}

	return true;
}

void TetrisGame::determineSecondsPerTick()
{
	if (score < 3000)
	{
		secondsPerTick = MAX_SECONDS_PER_TICK;
	}
	else if (score < 6000)
	{
		secondsPerTick = MIN_SECONDS_PER_TICK + ((MAX_SECONDS_PER_TICK - MIN_SECONDS_PER_TICK) * 0.9);
	}
	else if (score < 9000)
	{
		secondsPerTick = MIN_SECONDS_PER_TICK + ((MAX_SECONDS_PER_TICK - MIN_SECONDS_PER_TICK) * 0.8);
	}
	else if (score < 12000)
	{
		secondsPerTick = MIN_SECONDS_PER_TICK + ((MAX_SECONDS_PER_TICK - MIN_SECONDS_PER_TICK) * 0.7);
	}
	else if (score < 15000)
	{
		secondsPerTick = MIN_SECONDS_PER_TICK + ((MAX_SECONDS_PER_TICK - MIN_SECONDS_PER_TICK) * 0.6);
	}
	else if (score < 18000)
	{
		secondsPerTick = MIN_SECONDS_PER_TICK + ((MAX_SECONDS_PER_TICK - MIN_SECONDS_PER_TICK) * 0.5);
	}
	else if (score < 21000)
	{
		secondsPerTick = MIN_SECONDS_PER_TICK + ((MAX_SECONDS_PER_TICK - MIN_SECONDS_PER_TICK) * 0.4);
	}
	else if (score < 24000)
	{
		secondsPerTick = MIN_SECONDS_PER_TICK + ((MAX_SECONDS_PER_TICK - MIN_SECONDS_PER_TICK) * 0.3);
	}
	else if (score < 27000)
	{
		secondsPerTick = MIN_SECONDS_PER_TICK + ((MAX_SECONDS_PER_TICK - MIN_SECONDS_PER_TICK) * 0.2);
	}
	else if (score < 30000)
	{
		secondsPerTick = MIN_SECONDS_PER_TICK + ((MAX_SECONDS_PER_TICK - MIN_SECONDS_PER_TICK) * 0.1);
	}
	else
	{
		secondsPerTick = MIN_SECONDS_PER_TICK;
	}
}


// ======================================================
// ===================== Destructor =====================
// ======================================================
TetrisGame::~TetrisGame()
{
	if(pNextShapeHead)
	{
		do
		{
			NextShapes* pTemp{ pNextShapeHead->pNext };

			delete pNextShapeHead;

			pNextShapeHead = pTemp;
			
		} while (pNextShapeHead);
	}
}