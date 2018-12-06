#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include<QtGui>
#include "qpointfb.h"

class Algorithms
{
public:
    Algorithms() {};

    static int getPositionWinding(QPointFB q, std::vector<QPointFB> pol);
    static int getPointLinePosition(QPointFB &q, QPointFB &a, QPointFB &b);
    static double getTwoVectorsAngle(QPointFB &p1, QPointFB &p2, QPointFB &p3, QPointFB &p4);
    static T2LinesPosition get2LinesPosition(QPointFB &p1, QPointFB &p2, QPointFB &p3, QPointFB &p4, QPointFB &inters);
    void computePolygonIntersection(std::vector<QPointFB> &pol1, std::vector<QPointFB> &pol2);
};

#endif // ALGORITHMS_H
