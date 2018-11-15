#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include "edge.h"

#include <QtGui>
#include <vector>

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
    static int getNearestPoint(QPoint &p, std::vector<QPoint> points);
    static double getDistance(QPoint &p1, QPoint &p2);
    static int getDelaunayPoint(QPoint &s, QPoint &e, std::vector<QPoint> points);
    static std::vector<Edge> delaunayTriangulation(std::vector<QPoint> &points);
};

#endif // ALGORITHMS_H
