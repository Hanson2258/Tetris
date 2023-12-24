# Tetris - Recreating the Tetris game in C++
Orignally a school lab prooject, (V1.0,) future versions will be independent releases and improvements of the game.

This project uses the SFML library.

# How to play
If you would like to play the game, you can download a release version, unzip it, and run the exe file to play.

Please refer to each version's release notes to view the available controls.

## Version 1.0
(Based on school project.)

In release version 1.0, a barebone version of Tetris is available.

<p></p>
The game includes:

- Basic Tetris board (no grid lines)
- Viewing the next block (random block assignment)
- Basic controls
  - Rotate block to the right
  - Move Left, Right
  - Soft Drop (one row down,) Hard Drop (to the bottom)
- Basic score calculation
  - Soft Drop: 1 pts per row
  - Hard Drop: 2 pts * num rows
  - Single Row Clear: 100 pts
  - Double Row Clear: 300 pts
  - Triple Row Clear: 500 pts
  - Tetris: 800 pts

### Controls:
- Arrow Up: Rotate to the right
- Arrow Left: Move to the left
- Arrow Right: Move to the Right
- Arrow Down: Soft Drop
- Space: Hard Drop