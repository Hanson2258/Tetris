#include <iostream>
#include <SFML/Graphics.hpp>

#include "TetrisGame.h"


int main()
{
	// _CrtMemDumpAllObjectsSince(NULL); // For detecting memory leaks

	// Seeding the randomizer
	srand(static_cast<unsigned int>(time(nullptr)));

	// Declaring SFML sprite and textures
	sf::Sprite blockSprite;			// The Tetromino block sprite
	sf::Texture blockTexture;		// The Tetromino block texture
	sf::Sprite backgroundSprite;	// The Background sprite
	sf::Texture backgroundTexture;	// The Background texture

	// Load images
	backgroundTexture.loadFromFile("images/background_v2.0.png");	// Load the background image
	backgroundSprite.setTexture(backgroundTexture);							// Set background image

	blockTexture.loadFromFile("images/tiles.png");				// Load the Tetris block image
	blockSprite.setTexture(blockTexture);									// Set the Tetris sprite

	// Create the game window
	sf::RenderWindow window(sf::VideoMode(983, 799), "Tetris Game Window");

	window.setFramerateLimit(30); // Set a max frame rate of 30 FPS

	// Set pixel offset - Website to find pixel location: https://pixspy.com/
	const Point gameboardOffset{332, 135};				// The pixel offset of the top left of the game board 
	const Point holdShapeCenter{180, 238};				// The pixel center for the middle of the hold shape area
	const Point firstNextShapeCenterPoint{802, 239};	// The pixel center for the first next shape center of the next shapes area

	// An array of nextShapeCenters
	Point nextShapeCenter[TetrisGame::NUM_NEXT_SHAPES]{firstNextShapeCenterPoint};

	for (int i = 1; i < TetrisGame::NUM_NEXT_SHAPES; i++)
	{
		nextShapeCenter[i] = Point{802, nextShapeCenter[0].getY() + (TetrisGame::NEXT_SHAPE_Y_SPACE * i)};
	}


	// Set up a tetris game
	TetrisGame game(window, blockSprite, gameboardOffset, nextShapeCenter, holdShapeCenter);

	// Set up a clock so we can determine seconds per game loop
	sf::Clock clock;

	// Create an event for handling userInput from the GUI (graphical user interface)
	sf::Event guiEvent;

	// The main game loop
	while (window.isOpen())
	{
		// How long since the last loop (fraction of a second)		
		float elapsedTime = clock.getElapsedTime().asSeconds();
		clock.restart();

		// Handle any window or keyboard events that have occurred since the last game loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) // Handle close button clicked
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				game.onKeyPressed(event); // Handle key press
			}
		}

		game.processGameLoop(elapsedTime); // Handle tetris game logic in here.

		// Draw the game to the screen
		window.clear(sf::Color::White); // Clear the entire window
		window.draw(backgroundSprite);	// Draw the background (onto the window) 				
		game.draw();					// Draw the game (onto the window)
		window.display();				// Re-display the entire window
	}

	return 0;
}
