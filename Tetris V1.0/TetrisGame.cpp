#include "TetrisGame.h"

#include <cassert>
#include <thread>

// Initializing the static variables
const int TetrisGame::BLOCK_WIDTH{ 32 };
const int TetrisGame::BLOCK_HEIGHT{ 32 };
const double TetrisGame::MAX_SECONDS_PER_TICK{ 0.75 };
const double TetrisGame::MIN_SECONDS_PER_TICK{ 0.20 };


// ====================================
// ========= Member Functions =========
// ====================================

TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, const Point& gameboardOffset, const Point& nextShapeOffset)
    : window(window), blockSprite(blockSprite), gameboardOffset(gameboardOffset), nextShapeOffset(nextShapeOffset)
{
	reset();

	// Setup our font for drawing the score
	if (!scoreFont.loadFromFile("fonts/RedOctober.ttf"))
	{
		assert(false && "Missing font: RedOctober.ttf");
	}
	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(18);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(425, 325);
}


void TetrisGame::draw() const
{
	drawTetromino(currentShape, gameboardOffset);
	drawTetromino(nextShape, nextShapeOffset);
	drawGameboard();

	window.draw(scoreText);
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

		case sf::Keyboard::Space:
			score += drop(currentShape) * static_cast<int>(scoringActions::hardDrop);
			updateScoreDisplay();
			lock(currentShape);
			break;

		default:
			assert("Something extremely broken!");  // NOLINT(clang-diagnostic-string-conversion)
			break;
	}
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

	pickNextShape();
	spawnNextShape();

	pickNextShape();
}

void TetrisGame::pickNextShape()
{
	nextShape.setShape(Tetromino::getRandomShape());
}

bool TetrisGame::spawnNextShape()
{
	currentShape.setShape(nextShape.getShape());
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
	scoreText.setString("Score: " + std::to_string(score));
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