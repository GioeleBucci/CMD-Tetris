#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <time.h>

#include "points2D.h"

#define WIDTH 12
#define HEIGHT 22

#define FPS 6


typedef struct Game {
    /* The grid contains only the already placed tetrominos
     * not the falling one. The value contained in the 2d array
     * is the color of the single block (or AIR if empty) */
    int grid[HEIGHT][WIDTH];
} Game;

enum colors {
    AIR, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
};

int currentPieceRow, currentPieceCol;
int currentPieceType;


char pieces[7][4][4] = {
        { // Z
                "##  ",
                " ## ",
                "    ",
                "    "
        },
        { // S
                " ## ",
                "##  ",
                "    ",
                "    "
        },
        { // O
                "##  ",
                "##  ",
                "    ",
                "    "
        },
        { // J
                "#   ",
                "### ",
                "    ",
                "    "
        },
        { // T
                " #  ",
                "### ",
                "    ",
                "    "
        },
        { // I
                "    ",
                "####",
                "    ",
                "    "
        },
        { // L
                "  # ",
                "### ",
                "    ",
                "    "
        }
};

/* --------------------------------------------------------------------------------- */

/// Function to print the colored character
void printColoredChar(unsigned char ch, int pieceType) {
    printf("\x1b[3%dm%c\x1b[0m", (pieceType == 6 ? YELLOW : pieceType + 1), ch);
}

void generateNewTetromino(){
    currentPieceType = rand()%7;
    currentPieceRow = 1;
    currentPieceCol = WIDTH / 2 - 1;
}

void initGrid(Game *game) {

    for (int i = 0; i < HEIGHT; ++i)
        for (int j = 0; j < WIDTH; ++j)
            game->grid[i][j] = WHITE; // walls
    for (int i = 1; i < HEIGHT - 1; ++i)
        for (int j = 1; j < WIDTH - 1; ++j)
            game->grid[i][j] = AIR;

    //start random number generator
    time_t t;
    time(&t);
    srand((unsigned int)t);

    //generate first piece
    generateNewTetromino();
}

/// returns non-zero if a tetromino piece is rendered
int printTetromino(int i, int j, int type) {
    int row = i - currentPieceRow;
    int col = j - currentPieceCol;
    if (row < 4 && row >= 0 && col < 4 && col >= 0 && pieces[type][row][col] != ' ') {
        // since the terminal has no orange both O and L pieces use yellow
        printColoredChar(219, type);
        return 1;
    }
    return 0;
}

void refresh(Game *game) {
    system("cls");
    // PRINT GRID
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (!printTetromino(i, j, currentPieceType)) {
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

bool isCollision(const Game *game, int rowOffset, int colOffset, int pieceType) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            // found block
            if (pieces[pieceType][i][j] != ' ') {
                int row = currentPieceRow + i + rowOffset;
                int col = currentPieceCol + j + colOffset;
                if (game->grid[row][col] != AIR)
                    return true;
            }
        }
    }
    return false;
}

Point2D getInputs() {
    int input;
    if (kbhit())
        input = tolower(getch()); //kbhit() is a non-blocking input function
    Point2D dir;
    if (input == 'w') dir = newPoint2D(-1, 0);
    if (input == 'a') dir = newPoint2D(0, -1);
    if (input == 's') dir = newPoint2D(1, 0);
    if (input == 'd') dir = newPoint2D(0, 1);

    return dir;
}

void placeTetromino(Game *game, int type){
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if(pieces[type][i][j] != ' ')
                game->grid[currentPieceRow+i][currentPieceCol+j] = type;
        }
    }
}




int main() {
    Game game;
    initGrid(&game);
    refresh(&game);

    clock_t t;
    while (1/*currentPieceRow != HEIGHT - 3 */) {
        t = clock();
        fflush(stdin); //?
        Point2D input = getInputs();
        //check left or right movement
        if(isSamePoint2D(input,POINT2D_LEFT) && !isCollision(&game,0,-1,currentPieceType))
            currentPieceCol--;
        else if(isSamePoint2D(input,POINT2D_RIGHT) && !isCollision(&game,0,1,currentPieceType))
            currentPieceCol++;
        if (!isCollision(&game, 1, 0, currentPieceType)) {
            currentPieceRow++;
        }else{
            placeTetromino(&game,currentPieceType);
            generateNewTetromino();
        }
        t = clock() - t;
        double elapsedTime = ((double) t) / CLOCKS_PER_SEC * 1000;
        Sleep((1000 - (int) elapsedTime) / FPS);
        refresh(&game);
    }
}
