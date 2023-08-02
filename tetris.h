#ifndef TETRIS_TETRIS_H
#define TETRIS_TETRIS_H

#include "points2D.h"

#define WIDTH 12
#define HEIGHT 22

#define FPS 2

typedef struct Game {
    /* The grid contains only the already placed tetrominos
     * not the falling one. The value contained in the 2d array
     * is the color of the single block (or AIR if empty) */
    int grid[HEIGHT][WIDTH];
} Game;

enum colors {
    AIR, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
};

enum tetrominoes {
    PIECE_Z, PIECE_S, PIECE_O, PIECE_J, PIECE_T, PIECE_I, PIECE_L
};

extern int currentPieceRow, currentPieceCol;
extern int currentPieceType;
extern int currentPiece[4][4];
extern int bag[7]; // contains a permutation of the 7 pieces
extern int bagIndexNext; // 0 - 6, current element of the bag dropping

extern char pieces[7][4][4];


/* --------------------------------------------------------------------------------- */

/// Perform Fisher-Yates shuffle on pieces bag
void shuffleBag();

/// rotates all tetrominoes except the I piece
void rotateTetromino(int piece[4][4], int pieceType, bool clockwise);

/// Function to print the colored character
void printColoredChar(unsigned char ch, int pieceType);

/// generates a tetromino according to the current bagIndex
void generateNewTetromino();

void initGame(Game *game);

/// returns non-zero if a tetromino piece is rendered
int printTetromino(int i, int j, int type);

void refresh(Game *game);

bool isCollision(const Game *game, int piece[4][4], int rowOffset, int colOffset);

void placeTetromino(Game *game, int type);

Point2D getInputs(Game *game);

void debugPrintTetromino();

void printNext();

void addLineOn(Game *game, int row);

/// returns the number of lines cleared
int clearLines(Game *game);

int score(int linesCleared);

#endif //TETRIS_TETRIS_H
