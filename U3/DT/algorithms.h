#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>

typedef enum{
    LEFT = 0,
    RIGHT,
    ON
} TPosition;

class Algorithms
{
public:
    Algorithms();
    static TPosition getPointLinePosition(QPoint &q, QPoint &a, QPoint &b);
    static double getCircleRadius(QPoint &p1, QPoint &p2, QPoint &p3);
};

#endif // ALGORITHMS_H
