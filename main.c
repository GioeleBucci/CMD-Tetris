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

enum tetrominoes {
    PIECE_Z, PIECE_S, PIECE_O, PIECE_J, PIECE_T, PIECE_I, PIECE_L
};

int currentPieceRow, currentPieceCol;
int currentPieceType;
int currentPiece[4][4];
int bag[7]; // contains a permutation of the 7 pieces
int bagIndex = 0; // 0 - 6, current element of the bag dropping

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

/// Perform Fisher-Yates shuffle on pieces bag
void shuffleBag() {
    for (int i = 6; i >= 0; --i) {
        int x = rand() % (i + 1);
        int temp = bag[i];
        bag[i] = bag[x];
        bag[x] = temp;
    }
    printf("\n");
    for (int i = 0; i < 7; ++i) {
        printf("%d ", bag[i]);
    }
}

/// rotates all tetrominoes except the I piece
void rotateTetromino(int matrix[4][4], int pieceType, bool clockwise) {

    // to rotate a piece counterclockwise just rotate it clockwise 3 times
    for (int rotations = 0; rotations < (!clockwise ? 1 : 3); ++rotations) {

        // I piece rotation behaves differently from others
        if (pieceType == PIECE_I) {

            for (int i = 0; i < 4 / 2; i++) {
                for (int j = i; j < 4 - i - 1; j++) {
                    int temp = matrix[i][j];
                    matrix[i][j] = matrix[4 - 1 - j][i];
                    matrix[4 - 1 - j][i] = matrix[4 - 1 - i][4 - 1 - j];
                    matrix[4 - 1 - i][4 - 1 - j] = matrix[j][4 - 1 - i];
                    matrix[j][4 - 1 - i] = temp;
                }
            }

        } else {

            int temp[4][4];

            // Copy the original matrix to the temporary matrix
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    temp[i][j] = matrix[i][j];
                }
            }

            // Rotate the elements around the pivot in the temporary matrix
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    int x = i - 1;
                    int y = j - 1;
                    int new_x = -y;
                    int new_y = x;
                    int rotated_row = new_x + 1;
                    int rotated_col = new_y + 1;

                    // Check if the rotated position is within the matrix boundaries
                    if (rotated_row >= 0 && rotated_row < 4 && rotated_col >= 0 && rotated_col < 4) {
                        matrix[i][j] = temp[rotated_row][rotated_col];
                    }
                }
            }
        }
    }
}

/// Function to print the colored character
void printColoredChar(unsigned char ch, int pieceType) {
    printf("\x1b[3%dm%c\x1b[0m", (pieceType == 6 ? YELLOW : pieceType + 1), ch);
}

/// generates a tetromino according to the current bagIndex
void generateNewTetromino() {
    currentPieceType = bag[bagIndex];
    currentPieceRow = 1;
    currentPieceCol = WIDTH / 2 - 2;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            currentPiece[i][j] = pieces[currentPieceType][i][j];

    // if all the pieces of the bag are placed reshuffle the bag and restart the cycle
    if(bagIndex == 6){
        bagIndex = 0;
        shuffleBag();
    } else bagIndex++;
}

void initGame(Game *game) {

    for (int i = 0; i < HEIGHT; ++i)
        for (int j = 0; j < WIDTH; ++j)
            game->grid[i][j] = WHITE; // walls
    for (int i = 1; i < HEIGHT - 1; ++i)
        for (int j = 1; j < WIDTH - 1; ++j)
            game->grid[i][j] = AIR;

    //start random number generator
    time_t t;
    time(&t);
    srand((unsigned int) t);

    // generate bag and shuffle it
    for (int i = 0; i < 7; ++i)
        bag[i] = i;
    shuffleBag();
    bagIndex = 0;

    //generate first piece
    generateNewTetromino();
}

/// returns non-zero if a tetromino piece is rendered
int printTetromino(int i, int j, int type) {
    int row = i - currentPieceRow;
    int col = j - currentPieceCol;
    if (row < 4 && row >= 0 && col < 4 && col >= 0 && currentPiece[row][col] != ' ') {
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

bool isCollision(const Game *game, int rowOffset, int colOffset, int pieceType) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            // found block
            if (currentPiece[i][j] != ' ') {
                int row = currentPieceRow + i + rowOffset;
                int col = currentPieceCol + j + colOffset;
                if (game->grid[row][col] != AIR)
                    return true;
            }
        }
    }
    return false;
}

void placeTetromino(Game *game, int type) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (currentPiece[i][j] != ' ')
                game->grid[currentPieceRow + i][currentPieceCol + j] = type;
        }
    }
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
    if (input == 'z')
        //rotateMatrix(4, currentPiece);
        return dir;
}

void debugPrintTetromino() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%c", (currentPiece[i][j] == ' ' ? '-' : currentPiece[i][j]));
        }
        printf("\n");
    }
}

int main() {
    Game game;
    initGame(&game);
    refresh(&game);

    clock_t t;
    while (1) {
        t = clock();
        fflush(stdin); //?
        Point2D input = getInputs();
        //check left or right movement
        if (isSamePoint2D(input, POINT2D_LEFT) && !isCollision(&game, 0, -1, currentPieceType))
            currentPieceCol--;
        else if (isSamePoint2D(input, POINT2D_RIGHT) && !isCollision(&game, 0, 1, currentPieceType))
            currentPieceCol++;
        if (!isCollision(&game, 1, 0, currentPieceType)) {
            currentPieceRow++;
        } else {
            placeTetromino(&game, currentPieceType);
            generateNewTetromino();
        }
        t = clock() - t;
        double elapsedTime = ((double) t) / CLOCKS_PER_SEC * 1000;
        Sleep((1000 - (int) elapsedTime) / FPS);
        refresh(&game);
    }
}
