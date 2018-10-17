#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QtGui>
#include <vector>


class Algorithms
{
public:
    Algorithms();
    static int getPositionRay(QPoint q, std::vector<QPoint> pol);
};

#endif // ALGORITHMS_H
