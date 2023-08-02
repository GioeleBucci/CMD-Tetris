#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <time.h>

#include "points2D.h"
#include "tetris.h"

/* --------------------------------------------------------------------------------- */

int main() {
    Game game;
    initGame(&game);
    refresh(&game);
    addLineOn(&game,HEIGHT - 2);
    addLineOn(&game,HEIGHT - 3);
    game.grid[HEIGHT - 3][4] = BLUE;
    refresh(&game);
    getch();
    int x = clearLines(&game);
    refresh(&game);
    printf("\ncleared %d lines",x);
    //exit(1);
    clock_t t;
    while (1) {
        t = clock();
        fflush(stdin); //?
        Point2D input = getInputs(&game);
        //check left or right movement
        if (isSamePoint2D(input, POINT2D_LEFT) && !isCollision(&game, currentPiece, 0, -1))
            currentPieceCol--;
        else if (isSamePoint2D(input, POINT2D_RIGHT) && !isCollision(&game, currentPiece, 0, 1))
            currentPieceCol++;
        if (!isCollision(&game, currentPiece, 1, 0)) {
            currentPieceRow++;
        } else {
            placeTetromino(&game, currentPieceType);
            generateNewTetromino();
        }
        t = clock() - t;
        double elapsedTime = ((double) t) / CLOCKS_PER_SEC * 1000;
        Sleep((1000 - (int) elapsedTime) / FPS);
        refresh(&game);
        printNext();
    }
}
