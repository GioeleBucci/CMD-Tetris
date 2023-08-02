#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <time.h>
#include <assert.h>

#include "points2D.h"
#include "tetris.h"

int FPS = 24;

char input = '\0';

/* --------------------------------------------------------------------------------- */

void handleInput(Game *game) {

    if(input == '\0')
        return;

    //check left or right movement
    if (input == 'a' && !isCollision(game, currentPiece, 0, -1))
        currentPieceCol--;
    else if (input == 'd' && !isCollision(game, currentPiece, 0, 1))
        currentPieceCol++;

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

    // hard drop
    /* In the original game if the piece was hard dropped, an amount equal
     * to the number of rows covered by the hard drop plus 1 is added to the score */
    if (input == ' ') {
        game->score++;
        while (!isCollision(game, currentPiece, 1, 0)) {
            currentPieceRow++;
            game->score++;
        }
    }

    // reset input buffer
    input = '\0';
}

// Function to be executed by the new thread
void *inputGetter(Game *game) {
    while (1) {
        if (kbhit()) {
            input = tolower(getch());
        }
        Sleep(10);
        handleInput(game);
    }
}

int main() {

    Game game;
    initGame(&game);

    HANDLE threadHandle;
    DWORD threadId;

    // Create a new thread that will run the updateInputThread function
    threadHandle = CreateThread(NULL, 0, inputGetter, &game, 0, &threadId);
    assert(threadHandle != NULL);

    clock_t t;
    while (!gameOver(&game)) {

        t = clock();

        handleInput(&game);

        t = clock() - t;

        double elapsedTime = ((double) t) / CLOCKS_PER_SEC * 1000;
        Sleep((1000 - (int) elapsedTime) / FPS);
        update(&game);
    }

    printf("\033[%d;1H\n", HEIGHT + 1);
    puts("Game Over");
    CloseHandle(threadHandle);
    return 0;
}
