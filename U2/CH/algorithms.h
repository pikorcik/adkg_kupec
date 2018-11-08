#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QtGui>
using namespace std;
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
    static double get2LinesAngle(QPoint &p1,QPoint &p2,QPoint &p3, QPoint &p4);
    static double getPointLineDistance(QPoint &q, QPoint &a, QPoint &b);
    static QPolygon CHJarvis(vector<QPoint> &points);
    static QPolygon QHull(vector<QPoint> &points);
    static void qh_loc(int s, int e, vector<QPoint> &ss, QPolygon &h);
    static QPolygon CHSweepLine(vector<QPoint> &points);
};

#endif // ALGORITHMS_H
