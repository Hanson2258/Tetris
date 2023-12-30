# Tetris - Recreating the Tetris game in C++
Orignally a school lab prooject, (v1.0,) future versions will be independent releases and improvements of the game.

This project uses the SFML library.

# How to play
If you would like to play the game, you can download a release version, unzip it, and run the exe file to play.

Please refer to each version's release notes to view the available controls.

## Version 2.0
In release version 2.0, the Tetris game has been vastly upgraded to include many more features:

### Changelog
1. Background and interface improved
2. Added holding shape
3. Added ability to view next 3 shapes
4. Added "ghost shape"
5. Added leveling system
6. Added music
7. Added short delay after block placed
8. Improved shape selection process
    -  Must get each shape one time before getting the same shape
9. Improved scroring and timing system
10. Added pause after game end
11. Improved general look and feel

### Controls
- Arrow Up: Rotate to the right
- Arrow Left: Move to the left
- Arrow Right: Move to the Right
- Arrow Down: Soft Drop
- Space: Hard Drop
- C: Hold shape


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