#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <vector>
#include <QtGui>


class Algorithms
{
public:
    Algorithms();
    static int getPositionRay(QPoint q, std::vector<QPoint> pol);
    static int getPositionWinding(QPoint q, std::vector<QPoint> pol);
    static int getPointLinePosition(double xq, double yq, double x1, double y1, double x2, double y2);
    static double getTwoVectorsAngle(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
};

#endif // ALGORITHMS_H
