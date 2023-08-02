#include "tetris.h"
#include "points2D.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h>


 int currentPieceRow, currentPieceCol;
 int currentPieceType;
 int currentPiece[4][4];
 int bag[7]; // contains a permutation of the 7 pieces
 int bagIndexNext; // 0 - 6, current element of the bag dropping

extern char pieces[7][4][4] = {
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
                " ## ",
                " ## ",
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



/// Perform Fisher-Yates shuffle on pieces bag
void shuffleBag() {
    for (int i = 6; i >= 0; --i) {
        int x = rand() % (i + 1);
        int temp = bag[i];
        bag[i] = bag[x];
        bag[x] = temp;
    }
}

/// rotates all tetrominoes except the I piece
void rotateTetromino(int piece[4][4], int pieceType, bool clockwise) {

    // O piece doesn't rotate
    if (pieceType == PIECE_O)
        return;

    // to rotate a piece counterclockwise just rotate it clockwise 3 times
    for (int rotations = 0; rotations < (!clockwise ? 1 : 3); ++rotations) {

        // I piece rotation behaves differently from others
        if (pieceType == PIECE_I) {

            for (int i = 0; i < 4 / 2; i++) {
                for (int j = i; j < 4 - i - 1; j++) {
                    int temp = piece[i][j];
                    piece[i][j] = piece[4 - 1 - j][i];
                    piece[4 - 1 - j][i] = piece[4 - 1 - i][4 - 1 - j];
                    piece[4 - 1 - i][4 - 1 - j] = piece[j][4 - 1 - i];
                    piece[j][4 - 1 - i] = temp;
                }
            }

        } else {

            int temp[4][4];

            // Copy the original matrix to the temporary matrix
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    temp[i][j] = piece[i][j];
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
                        piece[i][j] = temp[rotated_row][rotated_col];
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
    currentPieceType = bag[bagIndexNext];
    currentPieceRow = 1;
    currentPieceCol = WIDTH / 2 - 2;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            currentPiece[i][j] = pieces[currentPieceType][i][j];

    // if all the pieces of the bag are placed reshuffle the bag and restart the cycle
    if (bagIndexNext == 6) {
        bagIndexNext = 0;
        shuffleBag();
    } else bagIndexNext++;
}

void initGame(Game *game) {

    game->score = 0;

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
    bagIndexNext = 0;

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

    // clears eventual lines and increases the score
    game->score+= score((clearLines(game)));

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

bool isCollision(const Game *game, int piece[4][4], int rowOffset, int colOffset) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            // found block
            if (piece[i][j] != ' ') {
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

Point2D getInputs(Game *game) {
    fflush(stdin);
    int input;
    if (kbhit())
        input = tolower(getch()); //kbhit() is a non-blocking input function
    Point2D dir;
    if (input == 'a') dir = newPoint2D(0, -1);
    if (input == 'd') dir = newPoint2D(0, 1);

    //handle rotations
    if (input == 'z' || input == 'x') {
        bool isClockwise = input == 'z' ? true : false;
        // clone piece, rotate it and check for collisions
        int newPiece[4][4];
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                newPiece[i][j] = currentPiece[i][j];
        rotateTetromino(newPiece, currentPieceType, isClockwise);
        if (!isCollision(game, newPiece, 0, 0))
            rotateTetromino(currentPiece, currentPieceType, isClockwise);
    }
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

void printNext() {
    int next = bag[bagIndexNext];
    int leftMargin = 4;
    /* \033[x;yH
     * is an ANSI escape code that moves the cursor at the x-th line and y-th row of the terminal
     * for eg printf("\033[%d;%dH", 4, 7) moves the cursor at the 7th character of the 4th line */

    // box top
    printf("\033[1;%dH", WIDTH + leftMargin);
    for (int i = 0; i < 7; ++i)
        printf("%c", 220);

    for (int i = 0; i < 4; ++i) {
        printf("\033[%d;%dH", i + 2, WIDTH + leftMargin);
        printf("%c ", 221); //left margin
        for (int j = 0; j < 4; ++j) {
            if (i == 0) printf(" ");
            else printColoredChar(pieces[next][i - 1][j] == ' ' ? ' ' : 219, next);
        }
        printf("%c", 222); //right margin
    }

    // box bottom
    printf("\033[%d;%dH", 6, WIDTH + leftMargin);
    for (int i = 0; i < 7; ++i)
        printf("%c", 223);
}


void addLineOn(Game *game, int row){
    for (int i = 1; i < WIDTH - 1; ++i) {
        game->grid[row][i] = BLUE;
    }
}

/// returns the number of lines cleared
int clearLines(Game *game){

    int linesCleared = 0;

    //start from bottom
    for (int i = HEIGHT - 2; i >= 1 ; --i) {
        bool isLineFull = true;
        for (int j = 1; j < WIDTH - 1; ++j) {
            if(game->grid[i][j] == AIR){
                isLineFull = false;
                break;
            }
        }
        if(isLineFull){
            linesCleared++;
            for (int j = i; j >= 2; --j) {
                for (int k = 1; k < WIDTH - 1; ++k) {
                    game->grid[j][k] = game->grid[j - 1][k];
                }
            }
            i++; // since everything shifted down check same line again
        }
    }
    return linesCleared;
}

/// returns score for lines cleared (40,100,300 or 1200)
int score(int linesCleared){
    // 40 100 300 1200
    if(linesCleared == 0)
        return 0;
    if(linesCleared == 1)
        return 40;
    if(linesCleared == 2)
        return 100;
    if(linesCleared == 3)
        return 300;
    if(linesCleared == 4) //tetris
        return 1200;
}
