#ifndef QPOINTFB_H
#define QPOINTFB_H

#include <QPointF>
#include "types.h"

class QPointFB : public QPointF
{
private:
    double alfa;
    double beta;
    bool intersection;
    TPointPolygon position;

public:
    QPointFB():QPointF(), alfa(0.0), beta(0.0), intersection(false), position(ON){};
    QPointFB(double x_, double y_, double alfa_, double beta_, bool intersection_, TPointPolygon position_):
        QPointF(x_, y_), alfa(alfa_), beta(beta_), intersection(intersection_), position(position_){};
    QPointFB(double x_, double y_): QPointF(x_, y_), alfa(0.0), beta(0.0), intersection(false), position(ON){};
    double getAlfa() {return alfa;}
    void setAlfa(double alfa_) {alfa = alfa_;}
    double getBeta() {return beta;}
    void setBeta(double beta_) {beta = beta_;}
    bool getIntersection() {return intersection;}
    void setIntersection(bool intersection_) {intersection = intersection_;}
    TPointPolygon getPosition() {return position;}
    void setPosition(TPointPolygon position_) {position = position_;}

};

#endif // QPOINTFB_H
