#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>
#include <qpointfb.h>

class Algorithms
{
public:
    Algorithms() {}

    static TPointPolygon getPositionWinding(QPointFB q, std::vector<QPointFB> pol);
    static TPointLinePosition getPointLinePosition(QPointFB &q, QPointFB &a, QPointFB &b);
    static double get2LinesAngle(QPointFB &p1,QPointFB &p2,QPointFB &p3, QPointFB &p4);
    static T2LinesPosition get2LinesPosition(QPointFB &p1,QPointFB &p2,QPointFB &p3, QPointFB &p4, QPointFB &intersection);
    static void computePolygonIntersections(std::vector<QPointFB> &p1, std::vector<QPointFB> &p2);
    static void processIntersection(QPointFB &b, double t, std::vector<QPointFB> &poly, int i);
    static void setPositions(std::vector<QPointFB> &pol1, std::vector<QPointFB> &pol2);
    static void createFragments(std::vector<QPointFB> &pol, TPointPolygon position, bool rev, std::map<QPointFB, std::vector<QPointFB> > &F);
    static void mergeFragments(std::map<QPointFB, std::vector<QPointFB> > &Fa, std::map<QPointFB, std::vector<QPointFB> > &Fb, std::vector<std::vector<QPointFB> > &C);
    static double getPolygonOrientation(std::vector<QPointFB> &pol);
    static std::vector<std::vector<QPointFB> > booleanOperation(std::vector<QPointFB> &pol1, std::vector<QPointFB> &pol2, TBooleanOperation oper);
};

#endif // ALGORITHMS_H
