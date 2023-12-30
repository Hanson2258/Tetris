#include "TetrisGame.h"

#include <cassert>
#include <chrono>
#include <thread>

#include "DebugNewOp.h"


// STATIC CONSTANTS INITIALIZATION ----------------------------------------
const int TetrisGame::BLOCK_WIDTH{ 32 };
const int TetrisGame::BLOCK_HEIGHT{ 32 };
const int TetrisGame::NEXT_SHAPE_Y_SPACE{ 95 };
const int TetrisGame::pauseTimeAfterShapePlaced{ 100 };

// STATIC VARIABLE INITIALIZATION -----------------------------------------
bool TetrisGame::holdShapeSet{ false };
bool TetrisGame::holdShapeSetThisRound{ false };

double TetrisGame::gameLoopTime[numLevels][numGameLoopElements]
{
	{15.974, gameLoopTime[ 0][0] / Gameboard::MAX_Y}, //  0
	{14.310, gameLoopTime[ 1][0] / Gameboard::MAX_Y}, //  1
	{12.646, gameLoopTime[ 2][0] / Gameboard::MAX_Y}, //  2
	{10.982, gameLoopTime[ 3][0] / Gameboard::MAX_Y}, //  3
	{ 9.318, gameLoopTime[ 4][0] / Gameboard::MAX_Y}, //  4
	{ 7.654, gameLoopTime[ 5][0] / Gameboard::MAX_Y}, //  5
	{ 5.990, gameLoopTime[ 6][0] / Gameboard::MAX_Y}, //  6
	{ 4.326, gameLoopTime[ 7][0] / Gameboard::MAX_Y}, //  7
	{ 2.662, gameLoopTime[ 8][0] / Gameboard::MAX_Y}, //  8
	{ 1.997, gameLoopTime[ 9][0] / Gameboard::MAX_Y}, //  9
	{ 1.664, gameLoopTime[10][0] / Gameboard::MAX_Y}, // 10
	{ 1.664, gameLoopTime[11][0] / Gameboard::MAX_Y}, // 11
	{ 1.664, gameLoopTime[12][0] / Gameboard::MAX_Y}, // 12
	{ 1.331, gameLoopTime[13][0] / Gameboard::MAX_Y}, // 13
	{ 1.331, gameLoopTime[14][0] / Gameboard::MAX_Y}, // 14
	{ 1.331, gameLoopTime[15][0] / Gameboard::MAX_Y}, // 15
	{ 0.998, gameLoopTime[16][0] / Gameboard::MAX_Y}, // 16
	{ 0.998, gameLoopTime[17][0] / Gameboard::MAX_Y}, // 17
	{ 0.998, gameLoopTime[18][0] / Gameboard::MAX_Y}, // 18
	{ 0.666, gameLoopTime[19][0] / Gameboard::MAX_Y}, // 19
	{ 0.666, gameLoopTime[20][0] / Gameboard::MAX_Y}, // 20
	{ 0.666, gameLoopTime[21][0] / Gameboard::MAX_Y}, // 21
	{ 0.666, gameLoopTime[22][0] / Gameboard::MAX_Y}, // 22
	{ 0.666, gameLoopTime[23][0] / Gameboard::MAX_Y}, // 23
	{ 0.666, gameLoopTime[22][0] / Gameboard::MAX_Y}, // 24
	{ 0.666, gameLoopTime[25][0] / Gameboard::MAX_Y}, // 25
	{ 0.666, gameLoopTime[26][0] / Gameboard::MAX_Y}, // 26
	{ 0.666, gameLoopTime[27][0] / Gameboard::MAX_Y}, // 27
	{ 0.666, gameLoopTime[28][0] / Gameboard::MAX_Y}, // 28
	{ 0.333, gameLoopTime[29][0] / Gameboard::MAX_Y}  // 29
};


// ========================================================================
// ============================= Constructor ==============================
// ========================================================================
TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, const Point& gameboardOffset,
                       const Point nextShapeCenter[], const Point& holdShapeCenter)
	: window(window), blockSprite(blockSprite), gameboardOffset(gameboardOffset),
	  holdShapeCenter(holdShapeCenter), nextShapeCenter(nextShapeCenter)
{
	// Setup our font for drawing the score
	if (!font.loadFromFile("fonts/times new roman.ttf"))
	{
		assert(false && "Missing font: times new roman.ttf");
	}
	
	audioSetup();	// Setup audio

	reset();		// Reset the game


	// Sets up the following for all sf::Text:
	//  - Text contents
	//  - Font
	//  - Character Size
	//  - Color
	//  - Position
	//
	// Including:
	//  - title
	//  - holdShapeTitle
	//  - nextShapeTitle
	//  - scoreTitle
	//  - scoreDisplay
	//  - levelTitle
	//  - levelDisplay
	//  - linesTitle
	//  - linesDisplay
	auto setupAllText = [this]()
	{
		// Title --------------------------------
		title.setString("Tetris v2.0");
		title.setFont(font);
		title.setCharacterSize(35);
		title.setFillColor(sf::Color::White);
		title.setPosition(491 - (title.getLocalBounds().width / 2), 75 - (title.getLocalBounds().height));


		// Hold ---------------------------------
		holdShapeTitle.setString("Hold");
		holdShapeTitle.setFont(font);
		holdShapeTitle.setCharacterSize(25);
		holdShapeTitle.setFillColor(sf::Color::White);
		holdShapeTitle.setPosition(180 - (holdShapeTitle.getLocalBounds().width / 2), 152 - (holdShapeTitle.getLocalBounds().height));


		// Next Shape ---------------------------
		nextShapeTitle.setString("Next Shape");
		nextShapeTitle.setFont(font);
		nextShapeTitle.setCharacterSize(25);
		nextShapeTitle.setFillColor(sf::Color::White);
		nextShapeTitle.setPosition(802 - (nextShapeTitle.getLocalBounds().width / 2), 152 - (nextShapeTitle.getLocalBounds().height));


		// Score Title --------------------------
		scoreTitle.setString("Score");
		scoreTitle.setFont(font);
		scoreTitle.setCharacterSize(25);
		scoreTitle.setFillColor(sf::Color::White);
		scoreTitle.setPosition(187 - (scoreTitle.getLocalBounds().width / 2), 468 - (scoreTitle.getLocalBounds().height));

		// Score Display
		scoreDisplay.setFont(font);
		scoreDisplay.setCharacterSize(18);
		scoreDisplay.setFillColor(sf::Color::White);
		scoreDisplay.setPosition(188 - (scoreDisplay.getLocalBounds().width / 2),
			502 - (scoreDisplay.getLocalBounds().height));


		// Level Title --------------------------
		levelTitle.setString("Level");
		levelTitle.setFont(font);
		levelTitle.setCharacterSize(25);
		levelTitle.setFillColor(sf::Color::White);
		levelTitle.setPosition(187 - (levelTitle.getLocalBounds().width / 2), 545 - (levelTitle.getLocalBounds().height));


		// Level Display
		levelDisplay.setFont(font);
		levelDisplay.setCharacterSize(18);
		levelDisplay.setFillColor(sf::Color::White);
		levelDisplay.setPosition(188 - (levelDisplay.getLocalBounds().width / 2),
			580 - (levelDisplay.getLocalBounds().height));


		// Lines (rows cleared) Title -----------
		linesTitle.setString("Lines");
		linesTitle.setFont(font);
		linesTitle.setCharacterSize(25);
		linesTitle.setFillColor(sf::Color::White);
		linesTitle.setPosition(187 - (linesTitle.getLocalBounds().width / 2), 622 - (linesTitle.getLocalBounds().height));

		// Lines (rows cleared) Display
		linesDisplay.setFont(font);
		linesDisplay.setCharacterSize(18);
		linesDisplay.setFillColor(sf::Color::White);
		linesDisplay.setPosition(188 - (linesDisplay.getLocalBounds().width / 2),
			656 - (linesDisplay.getLocalBounds().height));
	};

	setupAllText();	// Setup all text

}


// ========================================================================
// ============================ Public Methods ============================
// ========================================================================

void TetrisGame::onKeyPressed(const sf::Event& event)
{
	switch (event.key.code)
	{
	case sf::Keyboard::Up:
		if (attemptRotate(currentShape))
		{
			secondsSinceLastTick = 0;
			blockRotate.play();
		}

		break;

	case sf::Keyboard::Left:
		if (attemptMove(currentShape, -1, 0))
		{
			secondsSinceLastTick = 0;
		}
		break;

	case sf::Keyboard::Down:
		if (!attemptMove(currentShape, 0, 1))
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
		if (attemptMove(currentShape, 1, 0))
		{
			secondsSinceLastTick = 0;
		}
		break;

	case sf::Keyboard::Space:
		score += drop(currentShape) * static_cast<int>(scoringActions::hardDrop);
		updateScoreDisplay();
		lock(currentShape);
		blockDrop.play();

		break;

	case sf::Keyboard::C:
		setHoldShape();
		break;


	default:
		assert("Something extremely broken!"); // NOLINT(clang-diagnostic-string-conversion)
		break;
	}

	updateGhostShape();
}

void TetrisGame::processGameLoop(const float secondsSinceLastLoop)
{
	static bool needToPause = false;

	secondsSinceLastTick += secondsSinceLastLoop;
	secondsSinceLastPlacement += secondsSinceLastLoop;

	if ((secondsSinceLastTick > gameLoopTime[level - 1][1]) || (secondsSinceLastPlacement > gameLoopTime[level - 1][1]))
	{
		tick();
		secondsSinceLastTick = 0;
		secondsSinceLastPlacement = 0;
	}


	if (needToPause)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(pauseTimeAfterShapePlaced));
		needToPause = false;
	}


	if (shapePlacedSinceLastGameLoop)
	{
		holdShapeSetThisRound = false;
		needToPause = true;

		if (spawnNextShape())
		{
			pickNextShape();

			const int rowsCleared = board.removeCompletedRows();
			totalRowsCleared += rowsCleared;

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
			tetrisMusic.stop();
			gameOver.play();

			// 5 Second pause after game end
			std::this_thread::sleep_for(std::chrono::seconds(5));

			reset();
		}

		shapePlacedSinceLastGameLoop = false;
		updateLevel();
		updateLevelDisplay();
		updateLinesDisplay();
	}
}

void TetrisGame::draw() const
{
	// Draw Current and Ghost shape
	drawTetromino(currentShape, gameboardOffset);
	drawTetromino(ghostShape, gameboardOffset, 0.5f);

	// Draw Hold Shape if set
	if (holdShapeSet)
	{
		drawTetromino(holdShape, holdShapeOffset);
	}

	// Draw all Next Shape(s)
	NextShapes* pTemp = pNextShapeHead;
	for (int i = 0; i < NUM_NEXT_SHAPES; i++)
	{
		drawTetromino(pTemp->shape, nextShapeOffset[i]);

		pTemp = pTemp->pNext;
	}

	// Draw gameboard
	drawGameboard();

	// Draw all text
	window.draw(title);
	window.draw(holdShapeTitle);
	window.draw(nextShapeTitle);
	window.draw(scoreTitle);
	window.draw(scoreDisplay);
	window.draw(levelTitle);
	window.draw(levelDisplay);
	window.draw(linesTitle);
	window.draw(linesDisplay);
}


// ========================================================================
// =============================== Methods ================================
// ========================================================================

// ==============================================================
// ===================== Game loop methods ======================
// ==============================================================

void TetrisGame::tick()
{
	if (!attemptMove(currentShape, 0, 1))
	{
		lock(currentShape);
	}
}

void TetrisGame::updateLevel()
{
	if (level <= numLevels)
	{
		if (level != (totalRowsCleared / 10 + 1))
		{
			level = totalRowsCleared / 10 + 1;
			levelUp.play();
		}
	}
}

void TetrisGame::reset()
{
	// Reset and display Score, Level, and lines (rows cleared)
	score = 0;
	level = 1;
	totalRowsCleared = 0;
	updateScoreDisplay();
	updateLevelDisplay();
	updateLinesDisplay();

	// Clear gameboard
	board.empty();

	// Delete all shapes in nextShapes linked list
	deleteNextShapes();
	pNextShapeHead = nullptr;
	pNextShapeTail = nullptr;

	// Set hold shape to false
	holdShapeSet = false;

	// Pick & spawn next shape
	setStartingShapes();
	spawnNextShape();
	pickNextShape();

	// Play tetris music
	tetrisMusic.play();
}


// ==============================================================
// ========================= Set Shapes =========================
// ==============================================================

void TetrisGame::setStartingShapes()
{
	for (int i = 0; i < NUM_NEXT_SHAPES; i++)
	{
		NextShapes* pNextShape = addNewShape(!i);

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

TetrisGame::NextShapes* TetrisGame::addNewShape(const bool reset)
{
	const auto pNextShape = new NextShapes;
	pNextShape->shape.setShape(Tetromino::getRandomShape(reset));
	pNextShape->viewOffset = Point{
		static_cast<int>(pNextShape->shape.getXViewBlockOffset() * BLOCK_WIDTH),
		static_cast<int>(pNextShape->shape.getYViewBlockOffset() * BLOCK_HEIGHT)
	};
	pNextShape->pNext = nullptr;

	return pNextShape;
}

bool TetrisGame::spawnNextShape()
{
	currentShape.setShape(pNextShapeHead->shape.getShape());
	currentShape.setGridLoc(board.getSpawnLoc().getX(), board.getSpawnLoc().getY());
	updateGhostShape();

	return isPositionLegal(currentShape);
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
	pTemp = addNewShape(false);

	pNextShapeTail->pNext = pTemp;
	pNextShapeTail = pNextShapeTail->pNext;

	// Recalculate y-axis for all shapes
	pTemp = pNextShapeHead;
	for (int i = 0; i < NUM_NEXT_SHAPES; i++)
	{
		nextShapeOffset[i] = Point{
			nextShapeCenter[i].getX() - pTemp->viewOffset.getX(),
			nextShapeCenter[i].getY() - pTemp->viewOffset.getY()
		};

		pTemp = pTemp->pNext;
	}
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
			pickNextShape();
		}

		holdShapeOffset = Point{
			holdShapeCenter.getX() - static_cast<int>(holdShape.getXViewBlockOffset() * BLOCK_WIDTH),
			holdShapeCenter.getY() - static_cast<int>(holdShape.getYViewBlockOffset() * BLOCK_HEIGHT)
		};
		holdShapeSetThisRound = true;
	}
}

void TetrisGame::deleteNextShapes()
{
	if (pNextShapeHead)
	{
		do
		{
			NextShapes* pTemp{ pNextShapeHead->pNext };

			delete pNextShapeHead;

			pNextShapeHead = pTemp;
		} while (pNextShapeHead);
	}
}


// ==============================================================
// ========================== Movement ==========================
// ==============================================================

bool TetrisGame::attemptRotate(GridTetromino& shape) const
{
	GridTetromino tempTetromino{shape};
	tempTetromino.rotateClockwise();

	if (isPositionLegal(tempTetromino))
	{
		shape.rotateClockwise();

		return true;
	}

	return false;
}

bool TetrisGame::attemptMove(GridTetromino& shape, const int x, const int y) const
{
	GridTetromino tempTetromino{shape};
	tempTetromino.move(x, y);

	if (isPositionLegal(tempTetromino))
	{
		shape.move(x, y);

		return true;
	}

	return false;
}

int TetrisGame::drop(GridTetromino& shape) const
{
	int rowsDropped{0};

	while (attemptMove(shape, 0, 1))
	{
		rowsDropped++;
	}

	return rowsDropped;
}

void TetrisGame::lock(const GridTetromino& shape)
{
	const std::vector<Point> tetrominoLoc{shape.getBlockLocsMappedToGrid()};

	for (int i{0}; i < static_cast<int>(tetrominoLoc.size()); i++)
	{
		board.setContent(tetrominoLoc[i], static_cast<int>(shape.getColor()));
	}

	shapePlacedSinceLastGameLoop = true;
}

void TetrisGame::updateGhostShape()
{
	ghostShape = currentShape;
	drop(ghostShape);
}


// ==============================================================
// ================== State & gameplay/ logic ===================
// ==============================================================

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


// ==============================================================
// ========================== Graphics ==========================
// ==============================================================

void TetrisGame::drawBlock(const Point& topLeft, const int xOffset, const int yOffset, const Tetromino::TetColor& color,
                           const float alpha) const
{
	sf::Texture texture;
	texture.loadFromFile("./images/tiles.png", sf::IntRect(0, 0, BLOCK_WIDTH, BLOCK_HEIGHT));

	blockSprite.setTextureRect(sf::IntRect(0 + (static_cast<int>(color) * BLOCK_WIDTH), 0, BLOCK_WIDTH, BLOCK_HEIGHT));
	blockSprite.setPosition(static_cast<float>(topLeft.getX()) + static_cast<float>(BLOCK_WIDTH * xOffset),
	                        static_cast<float>(topLeft.getY()) + static_cast<float>(BLOCK_HEIGHT * yOffset));

	blockSprite.setColor(sf::Color(blockSprite.getColor().r, blockSprite.getColor().g, blockSprite.getColor().b,
	                               static_cast<sf::Uint8>(alpha * 255))); // 255 is the max alpha/ transparency, the lower the number, the more transparent the sprite

	window.draw(blockSprite);
}

void TetrisGame::drawGameboard() const
{
	for (int y{0}; y < Gameboard::MAX_Y; y++)
	{
		for (int x{0}; x < Gameboard::MAX_X; x++)
		{
			if (board.getContent(x, y) != Gameboard::EMPTY_BLOCK)
			{
				drawBlock(gameboardOffset, x, y, static_cast<Tetromino::TetColor>(board.getContent(x, y)));
			}
		}
	}
}

void TetrisGame::drawTetromino(const GridTetromino& tetromino, const Point& topLeft, float alpha) const
{
	const std::vector<Point> tetrominoLoc{tetromino.getBlockLocsMappedToGrid()};

	for (int i{0}; i < static_cast<int>(tetrominoLoc.size()); i++)
	{
		drawBlock(topLeft, tetrominoLoc[i].getX(), tetrominoLoc[i].getY(), tetromino.getColor(), alpha);
	}
}

void TetrisGame::updateScoreDisplay()
{
	scoreDisplay.setString(std::to_string(score));

	// Update score display location based on score
	scoreDisplay.setPosition(188 - (scoreDisplay.getLocalBounds().width / 2),
	                         502 - (scoreDisplay.getLocalBounds().height));
}

void TetrisGame::updateLevelDisplay()
{
	levelDisplay.setString(std::to_string(level));

	// Update level display location based on level
	levelDisplay.setPosition(188 - (levelDisplay.getLocalBounds().width / 2),
	                         579 - (levelDisplay.getLocalBounds().height));
}

void TetrisGame::updateLinesDisplay()
{
	linesDisplay.setString(std::to_string(totalRowsCleared));

	// Update score display location based on score
	linesDisplay.setPosition(188 - (linesDisplay.getLocalBounds().width / 2),
	                         656 - (linesDisplay.getLocalBounds().height));
}


// ==============================================================
// =========================== Audio ============================
// ==============================================================

void TetrisGame::audioSetup()
{
	if (!tetrisMusic.openFromFile("sfx/tetrisMusic.ogg"))
	{
		assert(false && "Missing audio file: tetrisMusic.ogg");
	}
	tetrisMusic.setVolume(5.f);

	if (!blockDrop.openFromFile("sfx/blockDrop.ogg"))
	{
		assert(false && "Missing audio file: blockDrop.ogg");
	}
	blockDrop.setVolume(35.f);

	if (!blockRotate.openFromFile("sfx/blockRotate.ogg"))
	{
		assert(false && "Missing audio file: blockRotate.ogg");
	}
	blockRotate.setVolume(15.f);

	if (!levelUp.openFromFile("sfx/levelUp.ogg"))
	{
		assert(false && "Missing audio file: levelUp.ogg");
	}
	levelUp.setVolume(25.f);

	if (!gameOver.openFromFile("sfx/gameOver.ogg"))
	{
		assert(false && "Missing audio file: gameOver.ogg");
	}
	gameOver.setVolume(20.f);
}


// ==============================================================
// ========================= Destructor =========================
// ==============================================================
TetrisGame::~TetrisGame()
{
	deleteNextShapes();
}
