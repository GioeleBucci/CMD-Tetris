#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#include "points2D.h"

#define WIDTH 12
#define HEIGHT 22


typedef struct Game {
    /* The grid contains only the already placed tetrominos
     * not the falling one. The value contained in the 2d array
     * is the color of the single block (or AIR if empty) */
    int grid[HEIGHT][WIDTH];
} Game;

enum colors {
    AIR, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
};

int currentPieceRow = 1, currentPieceCol = WIDTH / 2;

char pieces[7][4][4] = {
        { // I
                "    ",
                "####",
                "    ",
                "    "
        },
        { // J
                "#   ",
                "### ",
                "    ",
                "    "
        },
        { // L
                "  # ",
                "### ",
                "    ",
                "    "
        },
        { // O
                "##  ",
                "##  ",
                "    ",
                "    "
        },
        { // S
                " ## ",
                "##  ",
                "    ",
                "    "
        },
        { // T
                " #  ",
                "### ",
                "    ",
                "    "
        },
        { // Z
                "##  ",
                " ## ",
                "    ",
                "    "
        }
};

/* --------------------------------------------------------------------------------- */

/// Function to print the colored character
void printColoredChar(unsigned char ch, int color) {
    printf("\x1b[3%dm%c\x1b[0m", color, ch);
}

void initGrid(Game *game) {
    for (int i = 0; i < HEIGHT; ++i)
        for (int j = 0; j < WIDTH; ++j)
            game->grid[i][j] = WHITE; // walls
    for (int i = 1; i < HEIGHT - 1; ++i)
        for (int j = 1; j < WIDTH - 1; ++j)
            game->grid[i][j] = AIR;
}

/// returns non-zero if a tetromino piece is rendered
int printTetromino(int i, int j, int type) {
    int row = i - currentPieceRow;
    int col = j - currentPieceCol;
    if (row < 4 && row >= 0 && col < 4 && col>=0 && pieces[type][row][col] != ' ') {
        printColoredChar(pieces[type][row][col], BLUE);
        return 1;
    }
    return 0;
}

void refresh(Game *game) {

    // PRINT GRID
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (!printTetromino(i, j, 4)) {
                if (game->grid[i][j] == AIR)
                    printf(" ");
                else // in game->grid[i][j] is stored the color of the single block
                    printColoredChar(219, game->grid[i][j]);
            }
        }
        printf("\n");
    }

}

void placePieceAt(Game *game, int row, int col, int pieceType) {
    currentPieceRow = row;
    currentPieceCol = col;
}

void dropPiece() {
    //if (!isCollision(1, 0))
    //clearPiece();
    currentPieceRow++;
    //placePiece();
}

void fall(Game *game) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (game->grid[i][j] == '#') {
                game->grid[i + 1][j] = 'f';
            }
        }
    }
}


int main() {
    Game game;
    initGrid(&game);
    //generate a piece
    //placePieceAt(&game, 1, 1, 1);
    refresh(&game);
    //fall(&game);
}
