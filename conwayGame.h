//
// Created by szeke on 11/13/2022.
//

#ifndef NAGY_HAZI_CONWAYGAME_H
#define NAGY_HAZI_CONWAYGAME_H

#endif //NAGY_HAZI_CONWAYGAME_H

#include "gameEngine.h"

typedef struct Vector {
    int x;
    int y;
} Vector;

typedef struct GameState {
    int width;
    int height;
    bool* cells;
} GameState;

/**
 * Calculates the index of a cell based on a 2d vector
 *
 * @param coords - the coordinates of the cell
 * @param width  - the width of the gameState
 * @return       - the index of the cell in the GameState
 */
int getCellIndex(Vector coords, int width);

/**
 * Handles the dynamic memory allocation for the GameState
 *
 * @param width     - width of the game
 * @param height    - height of the game
 * @return          - a new GameState with dynamic memory
 */
GameState* createNewState(int width, int height);

/**
 * Destroys a gameState by freeing up its memory
 *
 * @param game  - the GameState
 */
void destroyGameState(GameState* state);

/**
 * Sets all cells to false
 *
 * @param game  - the GameState
 */
void clearCells(GameState* game);

/**
 * Sets all cells to a random state
 *
 * @param game  - the GameState
 */
void randomizeCells(GameState* game);

/**
 * Count the number of alive neighbours in 3x3 square around the cell
 *
 * @param pos   - the position Vector of the cell
 * @param game  - the GameState
 * @return      - the number of alive neighbours in a 3x3 area around the cell
 */
int countAliveNeighbours(Vector* pos, GameState* game);

/**
 * Calculates the next state of the cells
 *
 * @param game  - the GameState
 * @return      - the GameState containing the next state
 */
GameState* calculateNextState(GameState* game);

/**
 * Handles the dynamic memory management function calls when stepping the game
 * Creates a new state then steps then destroys the old state
 *
 * @param game  - the GameState
 */
void stepGame(GameState** game);

/**
 * Converts the booleans stored in the cells array of a GameState and
 * stores it in a given buffer
 *
 * @param game  - the GameState
 * @param buff  - the buffer to store the characters
 */
void convertToChar(GameState* game, char* buff);

/**
 * Loads a the state from a file into the GameState
 *
 * @param fileName  - filename containing a state
 * @param game      - the gameState
 */
void loadStateFromFile(char* fileName, GameState* game);

/**
 * Saves the GameState to file
 *
 * @param fileName  - filename to save to
 * @param game      - the gameState
 */
void saveStateToFile(char* fileName, GameState* game);
