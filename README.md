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
    -  Background image credits: https://www.pinterest.ca/pin/180425528807729969/
 2. Added holding shape
 3. Added ability to view next 3 shapes
 4. Added "ghost shape"
 5. Added leveling system (increases one level per 10 rows cleared)
 6. Added music
 7. Added short delay after block placed
 8. Improved shape selection process
    -  Must get each shape one time before getting the same shape
 9. Improved scroring and timing system
10. Added pause after game end
11. Improved general look and feel

### Scoring
| Action           | Score            |
|------------------|------------------|
|  Soft Drop       | 1 pts per row    |
| Hard Drop        | 2 pts * num rows |
| Single Row Clear | 100 pts * Level  |
| Double Row Clear | 300 pts * Level  |
| Triple Row Clear | 500 pts * Level  |
| Tetris           | 800 pts * Level  |

## Timing
Note: A tick is the amount of time before the game automatically moved the shape down a row

Scoring is based on the NTSC version of Tetris: <br>
https://listfist.com/list-of-tetris-levels-by-speed-nes-ntsc-vs-pal

| Level | Seconds per Round | Seconds per Tick |
|-------|-------------------|------------------|
|  1    | 15.974            | 0.841            |
|  2    | 14.310            | 0.753            |
|  3    | 12.646            | 0.666            |
|  4    | 10.982            | 0.578            |
|  5    |  9.318            | 0.490            |
|  6    |  7.654            | 0.403            |
|  7    |  5.990            | 0.315            |
|  8    |  4.326            | 0.228            |
|  9    |  2.662            | 0.140            |
| 10    |  1.997            | 0.105            |
| 11    |  1.664            | 0.088            |
| 12    |  1.664            | 0.088            |
| 13    |  1.664            | 0.088            |
| 14    |  1.331            | 0.070            |
| 15    |  1.331            | 0.070            |
| 16    |  1.331            | 0.070            |
| 17    |  0.998            | 0.053            |
| 18    |  0.998            | 0.053            |
| 19    |  0.998            | 0.053            |
| 20    | 0.666             | 0.035            |
| 21    | 0.666             | 0.035            |
| 22    | 0.666             | 0.035            |
| 23    | 0.666             | 0.035            |
| 24    | 0.666             | 0.035            |
| 25    | 0.666             | 0.035            |
| 26    | 0.666             | 0.035            |
| 27    | 0.666             | 0.035            |
| 28    | 0.666             | 0.035            |
| 29    | 0.666             | 0.035            |
| 30    | 0.333             | 0.018            |

### Controls
| Control     | Action              |
|-------------|---------------------|
| Arrow Up    | Rotate to the right |
| Arrow Left  | Move to the left    |
| Arrow Right | Move to the Right   |
| Arrow Down  | Soft Drop           |
| Space       | Hard Drop           |
| C           | Hold Shape          |



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