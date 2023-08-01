#ifndef SNAKEGAME_POINTS2D_H
#define SNAKEGAME_POINTS2D_H

#include <stdbool.h>

typedef struct Point2D {
    int yRow, xCol;
} Point2D;

#define POINT2D_UP newPoint2D(-1,0)
#define POINT2D_DOWN newPoint2D(1,0)
#define POINT2D_LEFT newPoint2D(0,-1)
#define POINT2D_RIGHT newPoint2D(0,1)

Point2D sumPoint2D(Point2D a, Point2D b);

Point2D newPoint2D(int y, int x);

void changePoint2D(Point2D *old, Point2D new);

bool isSamePoint2D(Point2D a, Point2D b);

void nullPoint2D(Point2D *point2D);

#endif
